#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"

struct block;

typedef bool (*collision_handler)(struct ball *ball, struct block block, int block_x, int block_y, enum axis axis);

struct block {
	sprite_t sprite;
	struct color particle_color;
	collision_handler collision_handler;
};

extern struct block BLOCK_GRASS;

void init_blocks();

void unload_blocks();

