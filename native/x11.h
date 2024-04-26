#ifndef NATIVE_X11_H
#define NATIVE_X11_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

#include "../assets/icon-64.xpm"
#define SPRITE_ICON icon_64

#include "../assets/ball.xpm"
#define SPRITE_BALL ball_xpm

#include "../assets/grass.xpm"
#define SPRITE_GRASS grass_xpm

#include "../assets/dirt.xpm"
#define SPRITE_DIRT dirt_xpm

#include "../assets/spike.xpm"
#define SPRITE_SPIKE spike_xpm

#include "../assets/small_spikes.xpm"
#define SPRITE_SMALL_SPIKES small_spikes_xpm

#include "../assets/rock.xpm"
#define SPRITE_ROCK rock_xpm

#include "../assets/font_5x7.xpm"
#define SPRITE_FONT_5x7 font_5x7_xpm

#include "../assets/font_5x7_x2.xpm"
#define SPRITE_FONT_5x7_x2 font_5x7_x2_xpm

#include "../assets/font_5x7_x4.xpm"
#define SPRITE_FONT_5x7_x4 font_5x7_x4_xpm

#include "../assets/logo.xpm"
#define SPRITE_LOGO logo_xpm

#include "../assets/background.xpm"
#define SPRITE_BACKGROUND background_xpm

typedef struct sprite {
	Pixmap pixmap;
	Pixmap shapemask;
	Picture picture;
	Picture shapemask_picture;
	int width;
	int height;
} sprite_t;

typedef char ** sprite_identifier_t;

#endif

