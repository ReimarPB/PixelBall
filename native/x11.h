#ifndef NATIVE_X11_H
#define NATIVE_X11_H

#include <X11/Xlib.h>

#include "../assets/icon.xpm"
#define SPRITE_ICON icon_xpm

#include "../assets/ball.xpm"
#define SPRITE_BALL ball_xpm

#include "../assets/grass.xpm"
#define SPRITE_GRASS grass_xpm

#include "../assets/font_5x7.xpm"
#define SPRITE_FONT_5x7 font_5x7_xpm

#include "../assets/font_5x7_x2.xpm"
#define SPRITE_FONT_5x7_x2 font_5x7_x2_xpm

#include "../assets/font_5x7_x4.xpm"
#define SPRITE_FONT_5x7_x4 font_5x7_x4_xpm

typedef struct sprite {
	Pixmap pixmap;
	Pixmap shapemask;
	int width;
	int height;
} sprite_t;

typedef char ** sprite_identifier_t;

#endif

