#include "../native/common.h"

typedef void (*check_collision_callback)(struct ball *ball, int block_x, int block_y);

struct block {
	sprite_identifier_t sprite;
	check_collision_callback check_collision;
};

extern struct block BLOCK_GRASS;

