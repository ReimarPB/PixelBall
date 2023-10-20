#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../scenes/game.h"
#include "../globals.h"
#include "../native/common.h"

struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE;
sprite_t sprite_grass, sprite_dirt, sprite_spike;

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

bool spike_collision_handler(struct ball *ball, struct block block, int block_x, int block_y, enum axis axis)
{
	die();
	return true;
}

void init_blocks(void)
{
	sprite_grass = load_sprite(SPRITE_GRASS);
	sprite_dirt = load_sprite(SPRITE_DIRT);
	sprite_spike = load_sprite(SPRITE_SPIKE);

	BLOCK_GRASS = (struct block) {
		.sprite = &sprite_grass,
		.particle_color = rgb(0, 211, 0),
		.has_shadow = true,
		.collision_handler = &standard_collision_handler,
	};

	BLOCK_DIRT = (struct block) {
		.sprite = &sprite_dirt,
		.particle_color = rgb(88, 68, 47),
		.has_shadow = true,
		.collision_handler = &standard_collision_handler,
	};

	BLOCK_SPIKE = (struct block) {
		.sprite = &sprite_spike,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &spike_collision_handler,
	};
}

void unload_blocks(void)
{
	unload_sprite(sprite_grass);
}

