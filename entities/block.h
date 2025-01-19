#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"
#include "ball.h"

typedef void collision_handler_t(int x, int y, void *block_state, struct ball *ball);

typedef void draw_function_t(int x, int y, void *block_state);

struct block {
	sprite_t *sprite;
	struct color particle_color;
	bool has_shadow;
	collision_handler_t *collision_handler;
	struct rectangle *hitbox;
	draw_function_t *draw_function;
	void *default_state;
	size_t default_state_size;
};

extern struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE, BLOCK_SMALL_SPIKES, BLOCK_ROCK, BLOCK_BUBBLE,
	BLOCK_JUMP_PAD;

void init_blocks(void);

void unload_blocks(void);

#endif

