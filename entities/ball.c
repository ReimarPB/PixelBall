#include <stddef.h>

#include "../native/common.h"
#include "../globals.h"
#include "../levels/level.h"
#include "../scenes/game.h"
#include "ball.h"
#include "block.h"
#include "particle.h"

sprite_t sprite_ball;

bool handle_block_collision(struct ball *ball, int block_x, int block_y, enum axis axis)
{
	if (block_x < 0 || block_x >= WIDTH_BLOCKS || block_y < 0 || block_y >= HEIGHT_BLOCKS) return false;

	struct block *block = level.blocks[block_y][block_x];

	if (!block || block->collision_handler) return false;

	switch (axis) {
		case X_AXIS:
			if (ball->x_vel > 0) ball->x_vel = -1;
			else ball->x_vel = 1;

			break;
		case Y_AXIS:

			if (ball->y_vel < 0) {
				ball->y_vel = 3.4;
			} else {
				ball->y_vel = -3.4;

				add_particle(block->particle_color, ball->x + BALL_SIZE / 2, block_y * BLOCK_SIZE, 0,  0.5, -0.4, -0.6);
				add_particle(block->particle_color, ball->x + BALL_SIZE / 2, block_y * BLOCK_SIZE, -0.5, 0, -0.4, -0.6);
			}

			break;
	}

	return true;
}

void init_ball(void)
{
	sprite_ball = load_sprite(SPRITE_BALL);
}

void unload_ball(void)
{
	unload_sprite(sprite_ball);
}

void update_ball(struct ball *ball, void *block_states[HEIGHT_PX][WIDTH_PX])
{
	int old_x = ball->x, old_y = ball->y;

	// Add velocity and check for block collisions

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
				handle_block_collision(ball, block_x, block_y_1, X_AXIS) ||
				handle_block_collision(ball, block_x, block_y_2, X_AXIS)
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
				handle_block_collision(ball, block_x_1, block_y, Y_AXIS) ||
				handle_block_collision(ball, block_x_2, block_y, Y_AXIS)
			) {
				ball->y = old_y;
			}
		}
	}

	// Check for hitbox collisions (used in special blocks)
	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			struct block *block = level.blocks[y][x];

			if (!block || !block->hitbox) continue;

			int hitbox_x_start = x * BLOCK_SIZE + block->hitbox->x;
			int hitbox_y_start = y * BLOCK_SIZE + block->hitbox->y;
			int hitbox_x_end = hitbox_x_start + block->hitbox->width;
			int hitbox_y_end = hitbox_y_start + block->hitbox->height;

			if (
				ball->x < hitbox_x_end && ball->x + BALL_SIZE > hitbox_x_start &&
				ball->y < hitbox_y_end && ball->y + BALL_SIZE > hitbox_y_start
			) {
				block->collision_handler(x, y, block_states[y][x], ball);
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

	if (ball->y > HEIGHT_PX) die();
}

void draw_ball(struct ball ball)
{
	draw_sprite(sprite_ball, (int)ball.x, (int)ball.y);
}

