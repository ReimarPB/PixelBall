#ifndef NATIVE_X11_H
#define NATIVE_X11_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

#include "../assets/icon-64.xpm"
#define SPRITE_ICON (sprite_identifier_t) { .xpm = icon_64_xpm, .opacity = 1.0 }

#include "../assets/ball.xpm"
#define SPRITE_BALL (sprite_identifier_t) { .xpm = ball_xpm, .opacity = 1.0 }

#include "../assets/grass.xpm"
#define SPRITE_GRASS (sprite_identifier_t) { .xpm = grass_xpm, .opacity = 1.0 }

#include "../assets/dirt.xpm"
#define SPRITE_DIRT (sprite_identifier_t) { .xpm = dirt_xpm, .opacity = 1.0 }

#include "../assets/spike.xpm"
#define SPRITE_SPIKE (sprite_identifier_t) { .xpm = spike_xpm, .opacity = 1.0 }

#include "../assets/small_spikes.xpm"
#define SPRITE_SMALL_SPIKES (sprite_identifier_t) { .xpm = small_spikes_xpm, .opacity = 1.0 }

#include "../assets/rock.xpm"
#define SPRITE_ROCK (sprite_identifier_t) { .xpm = rock_xpm, .opacity = 1.0 }

#include "../assets/bubble.xpm"
#define SPRITE_BUBBLE (sprite_identifier_t) { .xpm = bubble_xpm, .opacity = 0.1 }

#include "../assets/jump_pad_base.xpm"
#define SPRITE_JUMP_PAD_BASE (sprite_identifier_t) { .xpm = jump_pad_base_xpm, .opacity = 1.0 }

#include "../assets/jump_pad_platform.xpm"
#define SPRITE_JUMP_PAD_PLATFORM (sprite_identifier_t) { .xpm = jump_pad_platform_xpm, .opacity = 1.0 }

#include "../assets/font_5x7.xpm"
#define SPRITE_FONT_5x7 (sprite_identifier_t) { .xpm = font_5x7_xpm, .opacity = 1.0 }

#include "../assets/font_5x7_x2.xpm"
#define SPRITE_FONT_5x7_x2 (sprite_identifier_t) { .xpm = font_5x7_x2_xpm, .opacity = 1.0 }

#include "../assets/font_5x7_x4.xpm"
#define SPRITE_FONT_5x7_x4 (sprite_identifier_t) { .xpm = font_5x7_x4_xpm, .opacity = 1.0 }

#include "../assets/logo.xpm"
#define SPRITE_LOGO (sprite_identifier_t) { .xpm = logo_xpm, .opacity = 1.0 }

#include "../assets/background.xpm"
#define SPRITE_BACKGROUND (sprite_identifier_t) { .xpm = background_xpm, .opacity = 1.0 }

typedef struct sprite {
	Pixmap pixmap;
	Pixmap shapemask;
	Picture picture;
	Picture shapemask_picture;
	int width;
	int height;
} sprite_t;

typedef struct sprite_identifier {
	char **xpm;
	float opacity;
} sprite_identifier_t;

#endif

