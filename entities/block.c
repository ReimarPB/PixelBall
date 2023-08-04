#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../globals.h"
#include "../native/common.h"

struct block BLOCK_GRASS;
sprite_t sprite_grass;

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

void init_blocks()
{
	sprite_grass = load_sprite(SPRITE_GRASS);

	BLOCK_GRASS = (struct block) {
		.sprite = sprite_grass,
		.particle_color = rgb(0, 211, 0),
		.collision_handler = &standard_collision_handler,
	};
}

void unload_blocks()
{
	unload_sprite(sprite_grass);
}

