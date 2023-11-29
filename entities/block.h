#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"
#include "ball.h"

struct block;

typedef void collision_handler_t(void);

struct block {
	sprite_t *sprite;
	struct color particle_color;
	bool has_shadow;
	collision_handler_t *collision_handler;
	struct rectangle *hitbox;
};

extern struct block BLOCK_GRASS, BLOCK_DIRT, BLOCK_SPIKE, BLOCK_SMALL_SPIKES;

void init_blocks(void);

void unload_blocks(void);

#endif

