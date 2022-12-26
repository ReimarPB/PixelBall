#include "../native/common.h"
#include "ball.h"

void update_ball(struct ball *ball)
{
	ball->x += ball->x_vel;
	ball->y += ball->y_vel;

	ball->y_vel += 0.1;
}

void draw_ball(struct ball ball)
{
	sprite_t sprite_ball = load_sprite(SPRITE_BALL);
	draw_sprite(sprite_ball, ball.x, ball.y);
	unload_sprite(sprite_ball);
}

