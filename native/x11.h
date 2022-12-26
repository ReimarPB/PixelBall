#ifndef NATIVE_X11_H
#define NATIVE_X11_H

#include <X11/Xlib.h>

#include "../assets/ball.xpm"
#define SPRITE_BALL ball_xpm

#include "../assets/grass.xpm"
#define SPRITE_GRASS grass_xpm

typedef struct sprite {
	Pixmap pixmap;
	Pixmap shapemask;
	int width;
	int height;
} sprite_t;

typedef char ** sprite_identifier_t;

#endif

