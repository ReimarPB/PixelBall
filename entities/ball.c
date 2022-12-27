#include "../native/common.h"
#include "../globals.h"
#include "ball.h"

void update_ball(struct ball *ball)
{
	ball->x += ball->x_vel;
	ball->y += ball->y_vel;

	if (ball->going_left) ball->x_vel -= BALL_SPEED * 2;
	if (ball->going_right) ball->x_vel += BALL_SPEED * 2;

	// Make velocity approach zero when no keys held down
	if (ball->x_vel > 0)      ball->x_vel = MAX(ball->x_vel - BALL_SPEED, 0);
	else if (ball->x_vel < 0) ball->x_vel = MIN(ball->x_vel + BALL_SPEED, 0);

	if (ball->x_vel > BALL_MAX_SPEED) ball->x_vel = BALL_MAX_SPEED;
	if (ball->x_vel < -BALL_MAX_SPEED) ball->x_vel = -BALL_MAX_SPEED;

	ball->y_vel += 0.1;
}

void draw_ball(struct ball ball)
{
	sprite_t sprite_ball = load_sprite(SPRITE_BALL);
	draw_sprite(sprite_ball, (int)ball.x, (int)ball.y);
	unload_sprite(sprite_ball);
}

