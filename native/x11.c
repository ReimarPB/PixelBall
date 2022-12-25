#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "x11.h"
#include "../game.h"

Display *display;
Window window;

void init();
void draw();
void update();

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
	XClearArea(display, window, x, y, width, height, true);
	XFlush(display);
}

void *game_loop()
{
	while (true) {
		update();
		usleep(1.0 / FPS * 1000000);
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

	XSelectInput(display, window, ExposureMask);

	Atom deleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(display, window, &deleteMessage, 1);

	XMapWindow(display, window);
	
	pthread_t game_thread;
	pthread_create(&game_thread, NULL, game_loop, NULL);

	XEvent event;
	for (;;) {
		XNextEvent(display, &event);
		switch (event.type) {
			case Expose:
				draw();
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

