#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "x11.h"
#include "../globals.h"

Display *display;
Window window;

void init();
void draw(int x, int y, int width, int height);
void update();
void onkeydown(enum key key);
void onkeyup(enum key key);

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
}

void draw_sprite(sprite_t sprite, int x, int y)
{
	XGCValues values;
	values.clip_mask = sprite.shapemask;
	values.clip_x_origin = x;
	values.clip_y_origin = y;
	GC gc = XCreateGC(display, window, GCClipMask | GCClipXOrigin | GCClipYOrigin, &values);

	XCopyArea(display, sprite.pixmap, window, gc, 0, 0, sprite.width, sprite.height, x, y);
	XFreeGC(display, gc);
}

void draw_rect(int x, int y, int width, int height, int red, int green, int blue)
{
	GC gc = XCreateGC(display, window, 0, NULL);
	XSetForeground(display, gc, (red<<16) + (green<<8) + blue);
	XFillRectangle(display, window, gc, x, y, width, height);
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
	XFlush(display);
}

void *game_loop()
{
	while (true) {
		update();
		usleep(1.0 / FPS * 1000000);
	}
}

enum key translate_keycode(KeySym keysym)
{
	switch (keysym) {
		case XK_Left: return KEY_ARROW_LEFT;
		case XK_Right: return KEY_ARROW_RIGHT;
		case XK_Up: return KEY_ARROW_UP;
		case XK_Down: return KEY_ARROW_DOWN;
		default: return KEY_UNKNOWN;
	}
}

int main()
{
	XInitThreads();

	display = XOpenDisplay(NULL);

	unsigned long white = WhitePixel(display, DefaultScreen(display));
	unsigned long black = BlackPixel(display, DefaultScreen(display));

	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WIDTH_PX, HEIGHT_PX, 0, white, white);

	init();

	XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);

	Atom deleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(display, window, &deleteMessage, 1);

	XMapWindow(display, window);
	
	pthread_t game_thread;
	pthread_create(&game_thread, NULL, game_loop, NULL);

	XkbSetDetectableAutoRepeat(display, true, false);

	XEvent event;
	KeySym last_key = NULL;
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
				keysym = XKeycodeToKeysym(display, event.xkey.keycode, 0);
				if (keysym == last_key) break;

				if ((key = translate_keycode(keysym)))
					onkeydown(key);

				last_key = keysym;
				break;
			case KeyRelease:
				keysym = XKeycodeToKeysym(display, event.xkey.keycode, 0);

				if ((key = translate_keycode(keysym)))
					onkeyup(key);

				last_key = NULL;
				break;
			case ClientMessage:
				if (event.xclient.data.l[0] == deleteMessage) goto exit;
		}
	}

exit:
	pthread_cancel(game_thread);

	XCloseDisplay(display);

	return EXIT_SUCCESS;
}

