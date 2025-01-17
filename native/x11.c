#define SN_API_NOT_YET_FROZEN

#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/XKBlib.h>
#include <X11/extensions/Xdbe.h>
#include <X11/extensions/Xrender.h>
#include <libsn/sn.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "common.h"
#include "x11.h"
#include "../globals.h"
#include "../main.h"

Display *display;
Window window;

bool has_back_buffer = false;
XdbeBackBuffer back_buffer;

bool has_xrender = false;
Picture root_picture;
XRenderPictFormat *default_format;

bool has_drawn_to_screen = false;

int get_sprite_width(sprite_t sprite)
{
	return sprite.width;
}

int get_sprite_height(sprite_t sprite)
{
	return sprite.height;
}

unsigned long color_to_xcolor(struct color color)
{
	return (color.red << 16) + (color.green << 8) + color.blue;
}

XRenderColor color_to_xrendercolor(struct color color)
{
	return (XRenderColor) {
		.red = color.red / 255.0 * 0xFFFF,
		.green = color.green / 255.0 * 0xFFFF,
		.blue = color.blue / 255.0 * 0xFFFF,
		.alpha = color.alpha * 0xFFFF,
	};
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
	XWMHints hints = {
		.flags = IconPixmapHint | IconMaskHint,
		.icon_pixmap = icon.pixmap,
		.icon_mask = icon.shapemask,
	};

	XSetWMHints(display, window, &hints);
}

sprite_t load_sprite(sprite_identifier_t sprite_xpm)
{
	int width, height;
	sscanf(sprite_xpm[0], "%d %d ", &width, &height);

	XpmAttributes attributes;
	attributes.valuemask = XpmReturnAllocPixels | XpmReturnExtensions;

	Pixmap pixmap, shapemask = None;
	XpmCreatePixmapFromData(display, window, sprite_xpm, &pixmap, &shapemask, &attributes);

	Picture picture = None, shapemask_picture = None;
	if (has_xrender) {
		picture = XRenderCreatePicture(display, pixmap, default_format, 0, NULL);

		if (shapemask != None) shapemask_picture = XRenderCreatePicture(display, shapemask, XRenderFindStandardFormat(display, PictStandardA1), 0, NULL);
	}

	XpmFreeAttributes(&attributes);

	return (sprite_t) {
		.pixmap = pixmap,
		.shapemask = shapemask,
		.picture = picture,
		.shapemask_picture = shapemask_picture,
		.width = width,
		.height = height
	};
}

void unload_sprite(sprite_t sprite)
{
	XFreePixmap(display, sprite.pixmap);
	if (sprite.shapemask) XFreePixmap(display, sprite.shapemask);
	if (has_xrender) XRenderFreePicture(display, sprite.picture);
	if (sprite.shapemask_picture) XRenderFreePicture(display, sprite.shapemask_picture);
}

void draw_sprite(sprite_t sprite, int x, int y)
{
	draw_partial_sprite(sprite, x, y, 0, 0, sprite.width, sprite.height);
}

void draw_partial_sprite(sprite_t sprite, int x, int y, int sprite_x, int sprite_y, int sprite_width, int sprite_height)
{
	if (has_xrender) {
		XRenderComposite(
			display, PictOpOver, sprite.picture, sprite.shapemask_picture, root_picture,
			sprite_x, sprite_y, sprite_x, sprite_y,
			x, y, sprite_width, sprite_height
		);
	} else {
		XGCValues values;
		values.clip_mask = sprite.shapemask;
		values.clip_x_origin = x - sprite_x;
		values.clip_y_origin = y - sprite_y;

		GC gc = XCreateGC(display, window, GCClipMask | GCClipXOrigin | GCClipYOrigin, &values);

		XCopyArea(display, sprite.pixmap, back_buffer, gc, sprite_x, sprite_y, sprite_width, sprite_height, x, y);

		XFreeGC(display, gc);
	}

	has_drawn_to_screen = true;
}

void draw_rect(struct color color, int x, int y, int width, int height)
{
	if (has_xrender) {
		XRenderColor xrendercolor = color_to_xrendercolor(color);
		XRenderFillRectangle(display, PictOpOver, root_picture, &xrendercolor, x, y, width, height);
	} else {
		GC gc = XCreateGC(display, window, 0, NULL);

		XSetForeground(display, gc, color_to_xcolor(color));
		XFillRectangle(display, back_buffer, gc, x, y, width, height);

		XFreeGC(display, gc);
	}

	has_drawn_to_screen = true;
}

void redraw(void)
{
	redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
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

struct point get_mouse_coords(void)
{
	Window root, child;
	int root_x, root_y, win_x, win_y;
	unsigned int mask;

	if (XQueryPointer(display, window, &root, &child, &root_x, &root_y, &win_x, &win_y, &mask))
		return (struct point) { win_x, win_y };
	else
		return (struct point) { -1, -1 };
}

void *game_loop(void *arg)
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

int main(int argc, char **argv)
{
	XInitThreads();

	display = XOpenDisplay(NULL);

	unsigned long white = WhitePixel(display, DefaultScreen(display));
	unsigned long black = BlackPixel(display, DefaultScreen(display));

	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WIDTH_PX, HEIGHT_PX, 0, white, white);

	// Initialize startup notification
	SnDisplay *sn_display = sn_display_new(display, NULL, NULL);
	SnLauncheeContext *sn_context = sn_launchee_context_new_from_environment(sn_display, DefaultScreen(display));
	if (sn_context) sn_launchee_context_setup_window(sn_context, window);
	sn_display_unref(sn_display);

	// Xlib extensions

	// Create back buffer for double buffering
	XdbeSwapInfo swap_info;
	int xdbe_major_version, xdbe_minor_version;
	has_back_buffer = XdbeQueryExtension(display, &xdbe_major_version, &xdbe_minor_version);

	if (has_back_buffer) {
		printf("Using Xdbe version %d.%d\n", xdbe_major_version, xdbe_minor_version);

		back_buffer = XdbeAllocateBackBufferName(display, window, 0);

		swap_info = (XdbeSwapInfo) {
			.swap_window = window,
			.swap_action = 0
		};
	} else {
		puts("Xdbe extension not available. Output will not be double buffered.");

		back_buffer = window;
	}

	int xrender_major_version, xrender_minor_version;
	has_xrender = XRenderQueryVersion(display, &xrender_major_version, &xrender_minor_version);

	if (has_xrender) {
		printf("Using Xrender version %d.%d\n", xrender_major_version, xrender_minor_version);

		default_format = XRenderFindVisualFormat(display, DefaultVisual(display, 0));
		root_picture = XRenderCreatePicture(display, back_buffer, default_format, 0, NULL);
	} else puts("Xrender extension not available. Some visual effects might not show.");

	XWindowAttributes windowAttributes;
	XGetWindowAttributes(display, window, &windowAttributes);

	init();

	// Disable resizing
	XSizeHints size_hints = {
		.flags = PMinSize | PMaxSize,
		.min_width = WIDTH_PX,
		.max_width = WIDTH_PX,
		.min_height = HEIGHT_PX,
		.max_height = HEIGHT_PX,
	};
	XSetWMNormalHints(display, window, &size_hints);

	XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask);

	Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	XVisualInfo vinfo;
	XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

	XMapWindow(display, window);

	pthread_t game_thread;
	pthread_create(&game_thread, NULL, game_loop, NULL);

	XkbSetDetectableAutoRepeat(display, true, false);

	// Tell WM that the startup is completed
	if (sn_context) sn_launchee_context_complete(sn_context);

	XEvent event;
	KeySym last_key = XK_VoidSymbol;
	for (;;) {
		XNextEvent(display, &event);

		has_drawn_to_screen = false;
		enum key key;
		KeySym keysym;
		switch (event.type) {
			case Expose:
				if (event.xexpose.count != 0) break;

				XExposeEvent ex = event.xexpose;
				if (!ex.x && !ex.y && !ex.width && !ex.height) break;

				draw();
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
			case MotionNotify:
				onmousemove(event.xmotion.x, event.xmotion.y);
				break;
			case ButtonPress:
				onmousedown(event.xbutton.button, event.xbutton.x, event.xbutton.y);
				break;
			case ButtonRelease:
				onmouseup(event.xbutton.button, event.xbutton.x, event.xbutton.y);
				break;
			case ClientMessage:
				if (event.xclient.data.l[0] == wm_delete_window) goto exit;
		}

		if (has_back_buffer && has_drawn_to_screen) {
			XdbeSwapBuffers(display, &swap_info, 1);
		}
	}

exit:
	unload();

	pthread_cancel(game_thread);

	XCloseDisplay(display);

	return EXIT_SUCCESS;
}

