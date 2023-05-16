#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/XKBlib.h>
#include <X11/extensions/Xdbe.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "common.h"
#include "x11.h"
#include "../globals.h"

Display *display;
Window window;
XdbeBackBuffer back_buffer;
bool has_back_buffer = false;

void init();
void draw(int x, int y, int width, int height);
void update();
void onkeydown(enum key key, bool ctrl, bool alt, bool shift);
void onkeyup(enum key key, bool ctrl, bool alt, bool shift);
void unload();

unsigned long parse_color(struct color color)
{
	return (color.red << 16) + (color.green << 8) + color.blue;
}

struct color to_color(unsigned long color)
{
	return rgb((color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, color & 0xFF);
}

void set_window_title(char *title)
{
	XStoreName(display, window, title);
}

void set_window_icon(sprite_t icon)
{
	XWMHints *hints = XAllocWMHints();
	hints->flags = IconPixmapHint | IconMaskHint;
	hints->icon_pixmap = icon.pixmap;
	hints->icon_mask = icon.shapemask;

	XSetWMHints(display, window, hints);

	XFree(hints);
}

sprite_t load_sprite(sprite_identifier_t sprite)
{
	int width, height;
	sscanf(sprite[0], "%d %d ", &width, &height);

	XpmAttributes attributes;
	attributes.valuemask = XpmReturnAllocPixels | XpmReturnExtensions;

	Pixmap pixmap, shapemask;
	XpmCreatePixmapFromData(display, window, sprite, &pixmap, &shapemask, &attributes);

	XpmFreeAttributes(&attributes);

	sprite_t result = {
		.pixmap = pixmap,
		.shapemask = shapemask,
		.width = width,
		.height = height
	};
	return result;
}

void unload_sprite(sprite_t sprite)
{
	XFreePixmap(display, sprite.pixmap);
	if (sprite.shapemask) XFreePixmap(display, sprite.shapemask);
}

void draw_sprite(sprite_t sprite, int x, int y)
{
	XGCValues values;
	values.clip_mask = sprite.shapemask;
	values.clip_x_origin = x;
	values.clip_y_origin = y;
	GC gc = XCreateGC(display, window, GCClipMask | GCClipXOrigin | GCClipYOrigin, &values);

	if (brightness < 1) {
		XImage *image = XGetImage(display, sprite.pixmap, 0, 0, sprite.width, sprite.height, AllPlanes, ZPixmap);

		for (int x = 0; x < sprite.width; x++) {
			for (int y = 0; y < sprite.height; y++) {
				struct color color = to_color(XGetPixel(image, x, y));
				apply_brightness(&color, brightness);
				XPutPixel(image, x, y, parse_color(color));
			}
		}

		XPutImage(display, sprite.pixmap, DefaultGC(display, DefaultScreen(display)), image, 0, 0, 0, 0, sprite.width, sprite.height);
		XDestroyImage(image);
	}


	XCopyArea(display, sprite.pixmap, back_buffer, gc, 0, 0, sprite.width, sprite.height, x, y);
	XFreeGC(display, gc);
}

void draw_partial_sprite(sprite_t sprite, int x, int y, int sprite_x, int sprite_y, int sprite_width, int sprite_height)
{
	XGCValues values;
	values.clip_mask = sprite.shapemask;
	values.clip_x_origin = x - sprite_x;
	values.clip_y_origin = y - sprite_y;
	GC gc = XCreateGC(display, window, GCClipMask | GCClipXOrigin | GCClipYOrigin, &values);

	// TODO brightness

	XCopyArea(display, sprite.pixmap, back_buffer, gc, sprite_x, sprite_y, sprite_width, sprite_height, x, y);
	XFreeGC(display, gc);
}

void draw_rect(struct color color, int x, int y, int width, int height)
{
	GC gc = XCreateGC(display, window, 0, NULL);

	if (brightness < 1) apply_brightness(&color, brightness);

	XSetForeground(display, gc, parse_color(color));
	XFillRectangle(display, back_buffer, gc, x, y, width, height);

	XFreeGC(display, gc);
}

void redraw_area(int x, int y, int width, int height)
{
	XEvent event;
	memset(&event, 0, sizeof(event));
	event.type = Expose;
	event.xexpose.window = window;
	event.xexpose.x = x;
	event.xexpose.y = y;
	event.xexpose.width = width;
	event.xexpose.height = height;
	XSendEvent(display, window, false, ExposureMask, &event);
}

void *game_loop()
{
	struct timespec frame_time = {
		.tv_sec = 0,
		.tv_nsec = (1.0 / FPS) * 1000000000,
	};

	while (true) {
		update();
		XFlush(display);
		nanosleep(&frame_time, NULL);
	}
}

enum key translate_keycode(KeySym keysym)
{
	switch (keysym) {
		case XK_Left:   return KEY_ARROW_LEFT;
		case XK_Right:  return KEY_ARROW_RIGHT;
		case XK_Up:     return KEY_ARROW_UP;
		case XK_Down:   return KEY_ARROW_DOWN;
		case XK_Escape: return KEY_ESCAPE;
		case XK_Tab:    return KEY_TAB;
		case XK_Return: return KEY_ENTER;
		case XK_space:  return KEY_SPACE;
		default:        return KEY_UNKNOWN;
	}
}

int main()
{
	XInitThreads();

	display = XOpenDisplay(NULL);

	unsigned long white = WhitePixel(display, DefaultScreen(display));
	unsigned long black = BlackPixel(display, DefaultScreen(display));

	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WIDTH_PX, HEIGHT_PX, 0, white, white);

	// Create back buffer for double buffering
	XdbeSwapInfo swap_info;
	int xdbe_major_version, xdbe_minor_version;
	if (XdbeQueryExtension(display, &xdbe_major_version, &xdbe_minor_version)) {
		has_back_buffer = true;

		back_buffer = XdbeAllocateBackBufferName(display, window, 0);

		swap_info = (XdbeSwapInfo) {
			.swap_window = window,
			.swap_action = 0
		};
	}

	init();

	XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);

	Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	XVisualInfo vinfo;
	XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

	XMapWindow(display, window);
	
	pthread_t game_thread;
	pthread_create(&game_thread, NULL, game_loop, NULL);

	XkbSetDetectableAutoRepeat(display, true, false);

	XEvent event;
	KeySym last_key = XK_VoidSymbol;
	for (;;) {
		XNextEvent(display, &event);

		enum key key;
		KeySym keysym;
		switch (event.type) {
			case Expose:
				if (!event.xexpose.x && !event.xexpose.y && !event.xexpose.width && !event.xexpose.height) break;
				draw(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
				break;
			case KeyPress:
				keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
				if (keysym == last_key) break;

				if ((key = translate_keycode(keysym)))
					onkeydown(key, event.xkey.state & 4, event.xkey.state & 8, event.xkey.state & 1);

				last_key = keysym;
				break;
			case KeyRelease:
				keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);

				if ((key = translate_keycode(keysym)))
					onkeyup(key, event.xkey.state & 4, event.xkey.state & 8, event.xkey.state & 1);

				last_key = XK_VoidSymbol;
				break;
			case ClientMessage:
				if (event.xclient.data.l[0] == wm_delete_window) goto exit;
		}

		if (has_back_buffer) XdbeSwapBuffers(display, &swap_info, 1);
	}

exit:
	unload();

	pthread_cancel(game_thread);

	XCloseDisplay(display);

	return EXIT_SUCCESS;
}

