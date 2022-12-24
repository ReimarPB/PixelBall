#include "native/x11.h"
#include "native/common.h"

void start() {
	sprite_t sprite_ball = load_sprite(SPRITE_BALL);
	draw_sprite(sprite_ball, 0, 0);
}

