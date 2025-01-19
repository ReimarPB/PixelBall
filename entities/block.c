#include <math.h>

#include "ball.h"
#include "block.h"
#include "particle.h"
#include "../scenes/game.h"
#include "../globals.h"
#include "../native/common.h"

struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE, BLOCK_SMALL_SPIKES, BLOCK_ROCK, BLOCK_BUBBLE,
	BLOCK_JUMP_PAD;

sprite_t sprite_grass, sprite_dirt, sprite_spike, sprite_small_spikes, sprite_rock, sprite_bubble,
	sprite_jump_pad_base, sprite_jump_pad_platform;

struct rectangle spike_hitbox, small_spikes_hitbox, bubble_hitbox, jump_pad_hitbox;

void die_collision_handler(int x, int y, void *block_state, struct ball *ball)
{
	die();
}

// Bubble

bool default_bubble_state = false;

void draw_bubble(int x, int y, void *block_state)
{
	bool *burst = (bool *)block_state;

	if (*burst) return;

	// Make bubble animation effect
	double y_offset = 5 * (1 / (1 + pow(E, -2 * sin(ticks / 15.0)))) - 2;

	draw_sprite(sprite_bubble, x * BLOCK_SIZE, y * BLOCK_SIZE + y_offset);
}

void touch_bubble(int x, int y, void *block_state, struct ball *ball)
{
	bool *burst = (bool *)block_state;

	if (*burst) return;

	for (int i = 0; i < 10; i++) {
		add_particle(rgb(0, 235, 255), x * BLOCK_SIZE + BLOCK_SIZE / 2, (y + 1) * BLOCK_SIZE - BLOCK_SIZE / 2, -0.5, 0.5, -1.0, 1.0);
	}

	ball->y_vel = -3.6;

	*burst = true;
}

// Jump pad

int default_jump_pad_state = 0;

void draw_jump_pad(int x, int y, void *block_state)
{
	int *pushed_y = (int *)block_state;

	draw_partial_sprite(sprite_jump_pad_base, x * BLOCK_SIZE, y * BLOCK_SIZE + *pushed_y, 0, *pushed_y, BLOCK_SIZE, BLOCK_SIZE - *pushed_y);
	draw_sprite(sprite_jump_pad_platform, x * BLOCK_SIZE, y * BLOCK_SIZE + *pushed_y);
}

void jump_pad_collide(int x, int y, void *block_state, struct ball *ball)
{
	int *pushed_y = (int *)block_state;

	// Reset if jumping away
	if (ball->x + ball->x_vel > (x + 1) * BLOCK_SIZE || ball->x + ball->x_vel < x * BLOCK_SIZE - BALL_SIZE) {
		*pushed_y = 0;
	}

	// Ignore when hitting from the side
	if (*pushed_y == 0 && ball->y - ball->y_vel + BALL_SIZE > y * BLOCK_SIZE) {
		return;
	}

	if (ball->y_vel < 0 && *pushed_y > 0) {
		*pushed_y = MAX(0, *pushed_y + ball->y_vel);
	}

	if (ball->y_vel > 0) {
		*pushed_y = MIN(15, *pushed_y + ball->y_vel);
	}

	// Jump when hitting max pushed_y
	if (*pushed_y >= 15) {
		ball->y_vel = -5;
	}

	ball->y = y * BLOCK_SIZE + *pushed_y - BALL_SIZE;
}

void init_blocks(void)
{
	sprite_grass = load_sprite(SPRITE_GRASS);
	BLOCK_GRASS = (struct block) {
		.sprite = &sprite_grass,
		.particle_color = rgb(0, 211, 0),
		.has_shadow = true,
		.collision_handler = NULL,
		.hitbox = NULL,
		.draw_function = NULL,
		.default_state = NULL,
		.default_state_size = 0,
	};

	sprite_dirt = load_sprite(SPRITE_DIRT);
	BLOCK_DIRT = (struct block) {
		.sprite = &sprite_dirt,
		.particle_color = rgb(88, 68, 47),
		.has_shadow = true,
		.collision_handler = NULL,
		.hitbox = NULL,
		.draw_function = NULL,
		.default_state = NULL,
		.default_state_size = 0,
	};

	sprite_spike = load_sprite(SPRITE_SPIKE);
	spike_hitbox = (struct rectangle) { .x = 8, .y = 12, .width = 12, .height = 16 };
	BLOCK_SPIKE = (struct block) {
		.sprite = &sprite_spike,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &die_collision_handler,
		.hitbox = &spike_hitbox,
		.draw_function = NULL,
		.default_state = NULL,
		.default_state_size = 0,
	};

	sprite_small_spikes = load_sprite(SPRITE_SMALL_SPIKES);
	small_spikes_hitbox = (struct rectangle) { .x = 3, .y = 24, .width = 22, .height = 4 };
	BLOCK_SMALL_SPIKES = (struct block) {
		.sprite = &sprite_small_spikes,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &die_collision_handler,
		.hitbox = &small_spikes_hitbox,
		.draw_function = NULL,
		.default_state = NULL,
		.default_state_size = 0,
	};

	sprite_rock = load_sprite(SPRITE_ROCK);
	BLOCK_ROCK = (struct block) {
		.sprite = &sprite_rock,
		.particle_color = rgb(155, 155, 155),
		.has_shadow =  true,
		.collision_handler = NULL,
		.hitbox = NULL,
		.draw_function = NULL,
		.default_state = NULL,
		.default_state_size = 0,
	};

	sprite_bubble = load_sprite(SPRITE_BUBBLE);
	bubble_hitbox = (struct rectangle) { .x = 8, .y = 8, .width = 12, .height = 12 };
	BLOCK_BUBBLE = (struct block) {
		.sprite = &sprite_bubble,
		.particle_color = rgb(200, 250, 250),
		.has_shadow = false,
		.collision_handler = &touch_bubble,
		.hitbox = &bubble_hitbox,
		.draw_function = &draw_bubble,
		.default_state = &default_bubble_state,
		.default_state_size = sizeof(bool),
	};

	sprite_jump_pad_base = load_sprite(SPRITE_JUMP_PAD_BASE);
	sprite_jump_pad_platform = load_sprite(SPRITE_JUMP_PAD_PLATFORM);
	jump_pad_hitbox = (struct rectangle) { .x = 0, .y = 0, .width = 28, .height = 28 };
	BLOCK_JUMP_PAD = (struct block) {
		.sprite = NULL,
		.particle_color = NO_COLOR,
		.has_shadow = false,
		.collision_handler = &jump_pad_collide,
		.hitbox = &jump_pad_hitbox,
		.draw_function = &draw_jump_pad,
		.default_state = &default_jump_pad_state,
		.default_state_size = sizeof(int),
	};
}

void unload_blocks(void)
{
	unload_sprite(sprite_grass);
}

