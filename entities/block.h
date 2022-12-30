#include "../native/common.h"

struct block;

typedef void (*check_collision_callback)(struct ball *ball, struct block block, int block_x, int block_y);

struct block {
	sprite_identifier_t sprite;
	struct color particle_color;
	check_collision_callback check_collision;
};

extern struct block BLOCK_GRASS;

