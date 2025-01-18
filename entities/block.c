#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../scenes/game.h"
#include "../globals.h"
#include "../native/common.h"

struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE, BLOCK_SMALL_SPIKES, BLOCK_ROCK, BLOCK_BUBBLE;
sprite_t sprite_grass, sprite_dirt, sprite_spike, sprite_small_spikes, sprite_rock, sprite_bubble;

struct rectangle spike_hitbox = { .x = 8, .y = 12, .width = 12, .height = 16 };
struct rectangle small_spikes_hitbox = { .x = 3, .y = 24, .width = 22, .height = 4 };
struct rectangle bubble_hitbox = { .x = 8, .y = 8, .width = 12, .height = 12 };

void die_collision_handler(int x, int y)
{
	die();
}

void init_blocks(void)
{
	sprite_grass = load_sprite(SPRITE_GRASS);
	sprite_dirt = load_sprite(SPRITE_DIRT);
	sprite_spike = load_sprite(SPRITE_SPIKE);
	sprite_small_spikes = load_sprite(SPRITE_SMALL_SPIKES);
	sprite_rock = load_sprite(SPRITE_ROCK);
	sprite_bubble = load_sprite(SPRITE_BUBBLE);

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
		.collision_handler = &die_collision_handler,
		.hitbox = &spike_hitbox,
	};

	BLOCK_SMALL_SPIKES = (struct block) {
		.sprite = &sprite_small_spikes,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &die_collision_handler,
		.hitbox = &small_spikes_hitbox,
	};

	BLOCK_ROCK = (struct block) {
		.sprite = &sprite_rock,
		.particle_color = rgb(155, 155, 155),
		.has_shadow =  true,
		.collision_handler = NULL,
		.hitbox = NULL,
	};

	BLOCK_BUBBLE = (struct block) {
		.sprite = &sprite_bubble,
		.particle_color = rgb(200, 250, 250),
		.has_shadow = false,
		.collision_handler = &touch_bubble,
		.hitbox = &bubble_hitbox,
	};
}

void unload_blocks(void)
{
	unload_sprite(sprite_grass);
}

