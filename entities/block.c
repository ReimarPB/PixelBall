#include "ball.h"
#include "block.h"
#include "../globals.h"
#include "../native/common.h"

void standard_collision_checker(struct ball *ball, int block_x, int block_y)
{
	int ball_x_start = ball->x;
	int ball_x_end = ball->x + BALL_SIZE;
	int ball_y_start = ball->y;
	int ball_y_end = ball->y + BALL_SIZE;
	int block_x_start = block_x;
	int block_x_end = block_x + BLOCK_SIZE;
	int block_y_start = block_y;
	int block_y_end = block_y + BLOCK_SIZE;

	// Check collisions
	if (ball_y_start >= block_y_start && ball_y_start <= block_y_end) {
		// from left
		if (ball_x_end > block_x_start && ball_x_start < block_x_start) {
			ball->x = block_x_start - BALL_SIZE;
			ball->x_vel = 1;
		// from right
		} else if (ball_x_start < block_x_end && ball_x_end > block_x_end) {
			ball->x = block_x_end;
			ball->x_vel = 1;
		}
	}

	if (ball_x_start >= block_x_start && ball_x_start <= block_x_end) {
		// from top
		if (ball_y_end > block_y_start && ball_y_start < block_y_start) {
			ball->y = block_y - BALL_SIZE;
			ball->y_vel = -2.0;
		// from bottom
		} else if (ball_y_start < block_y_end && ball_y_end > block_y_end) {
			ball->y = block_y_end;
			ball->y_vel = 2.0;
		}
	}
}

struct block BLOCK_GRASS = {
	.sprite = SPRITE_GRASS,
	.check_collision = &standard_collision_checker,
};

