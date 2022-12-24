#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "x11.h"
#include "../game.h"

Display *display;
Window window;

void init();
void start();

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

void draw_sprite(sprite_t sprite, int x, int y)
{
	XGCValues values;
	values.clip_mask = sprite.shapemask;

	GC gc = XCreateGC(display, window, GCClipMask, &values);

	XCopyArea(display, sprite.pixmap, window, gc, 0, 0, sprite.width, sprite.height, x, y);

	XFreeGC(display, gc);
}

int main()
{
	display = XOpenDisplay(NULL);

	unsigned long white = WhitePixel(display, DefaultScreen(display));
	unsigned long black = BlackPixel(display, DefaultScreen(display));

	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WIDTH_PX, HEIGHT_PX, 0, white, white);

	init();

	XSelectInput(display, window, StructureNotifyMask);

	XMapWindow(display, window);

	GC gc = XCreateGC(display, window, 0, NULL);
	XSetForeground(display, gc, black);

	XEvent event;
	for (;;) {
		XNextEvent(display, &event);
		switch (event.type) {
			case MapNotify:
				start();
				break;
		}
	}

	XCloseDisplay(display);

	return EXIT_SUCCESS;
}

