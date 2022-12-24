#include <X11/Xlib.h>

#include "../assets/ball.xpm"
#define SPRITE_BALL ball_xpm

typedef struct sprite {
	Pixmap pixmap;
	Pixmap shapemask;
} sprite_t;

typedef char * sprite_identifier_t;

