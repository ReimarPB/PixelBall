#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../scenes/game.h"
#include "../globals.h"
#include "../native/common.h"

struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE;
sprite_t sprite_grass, sprite_dirt, sprite_spike;

struct rectangle hitbox = { .x = 8, .y = 12, .width = 12, .height = 16 };

void init_blocks(void)
{
	sprite_grass = load_sprite(SPRITE_GRASS);
	sprite_dirt = load_sprite(SPRITE_DIRT);
	sprite_spike = load_sprite(SPRITE_SPIKE);

	BLOCK_GRASS = (struct block) {
		.sprite = &sprite_grass,
		.particle_color = rgb(0, 211, 0),
		.has_shadow = true,
		.collision_handler = NULL,
		.hitbox = NULL,
	};

	BLOCK_DIRT = (struct block) {
		.sprite = &sprite_dirt,
		.particle_color = rgb(88, 68, 47),
		.has_shadow = true,
		.collision_handler = NULL,
		.hitbox = NULL,
	};

	BLOCK_SPIKE = (struct block) {
		.sprite = &sprite_spike,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &die,
		.hitbox = &hitbox,
	};
}

void unload_blocks(void)
{
	unload_sprite(sprite_grass);
}

