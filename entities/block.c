#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../globals.h"
#include "../native/common.h"

bool standard_collision_handler(struct ball *ball, struct block block, int block_x, int block_y, enum axis axis)
{
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

				add_particle(block.particle_color, ball->x + BALL_SIZE / 2, block_y * BLOCK_SIZE, 0,  0.5, -0.4, -0.6);
				add_particle(block.particle_color, ball->x + BALL_SIZE / 2, block_y * BLOCK_SIZE, -0.5, 0, -0.4, -0.6);
			}

			break;
	}

	return true;
}

struct block BLOCK_GRASS = {
	.sprite = SPRITE_GRASS,
	.particle_color = (struct color) { .red = 0, .green = 211, .blue = 0 },
	.collision_handler = &standard_collision_handler,
};

