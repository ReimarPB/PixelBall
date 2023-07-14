#include <stddef.h>

#include "../native/common.h"
#include "../globals.h"
#include "../scenes/game.h"
#include "ball.h"
#include "block.h"

sprite_t sprite_ball;

bool handle_collision(struct ball *ball, int block_x, int block_y, enum axis axis)
{
	if (block_x < 0 || block_x >= WIDTH_BLOCKS || block_y < 0 || block_y >= HEIGHT_BLOCKS) return false;

	struct block *block = level[block_y][block_x];
	return block != NULL && block->collision_handler(ball, *block, block_x, block_y, axis);
}

void init_ball()
{
	sprite_ball = load_sprite(SPRITE_BALL);
}

void unload_ball()
{
	unload_sprite(sprite_ball);
}

void update_ball(struct ball *ball)
{
	int old_x = ball->x, old_y = ball->y;

	// Add velocity and check for collisions

	ball->x += ball->x_vel;
	{
		int block_x, old_block_x;
		if (ball->x_vel < 0) {
			block_x =     (int)(ball->x / BLOCK_SIZE);
			old_block_x = (int)(old_x   / BLOCK_SIZE);
		} else if (ball->x_vel > 0) {
			block_x =     (int)((ball->x + BALL_SIZE) / BLOCK_SIZE);
			old_block_x = (int)((old_x   + BALL_SIZE) / BLOCK_SIZE);
		}

		if (block_x != old_block_x) {
			int block_y_1 = (int)(ball->y / BLOCK_SIZE);
			int block_y_2 = (int)((ball->y + BALL_SIZE) / BLOCK_SIZE);

			if (
				handle_collision(ball, block_x, block_y_1, X_AXIS) ||
				handle_collision(ball, block_x, block_y_2, X_AXIS)
			) {
				ball->x = old_x;
			}
		}
	}

	ball->y += ball->y_vel;
	{
		int block_y, old_block_y;
		if (ball->y_vel < 0) {
			block_y =     (int)(ball->y / BLOCK_SIZE);
			old_block_y = (int)(old_y   / BLOCK_SIZE);
		} else if (ball->y_vel > 0) {
			block_y =     (int)((ball->y + BALL_SIZE) / BLOCK_SIZE);
			old_block_y = (int)((old_y   + BALL_SIZE) / BLOCK_SIZE);
		}

		if (block_y != old_block_y) {
			int block_x_1 = (int)(ball->x / BLOCK_SIZE);
			int block_x_2 = (int)((ball->x + BALL_SIZE) / BLOCK_SIZE);

			if (
				handle_collision(ball, block_x_1, block_y, Y_AXIS) ||
				handle_collision(ball, block_x_2, block_y, Y_AXIS)
			) {
				ball->y = old_y;
			}
		}
	}

	if (ball->going_left) ball->x_vel -= BALL_SPEED * 2;
	if (ball->going_right) ball->x_vel += BALL_SPEED * 2;

	// Make velocity approach zero when no keys held down
	if (ball->x_vel > 0)      ball->x_vel = MAX(ball->x_vel - BALL_SPEED, 0);
	else if (ball->x_vel < 0) ball->x_vel = MIN(ball->x_vel + BALL_SPEED, 0);

	if (ball->x_vel > BALL_MAX_SPEED) ball->x_vel = BALL_MAX_SPEED;
	if (ball->x_vel < -BALL_MAX_SPEED) ball->x_vel = -BALL_MAX_SPEED;

	ball->y_vel += 0.15;
}

void draw_ball(struct ball ball)
{
	draw_sprite(sprite_ball, (int)ball.x, (int)ball.y);
}

