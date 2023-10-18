#include "../globals.h"
#include "../entities/block.h"

#define LOAD_LEVEL(name)                                        \
	extern const char _binary_levels_ ## name ## _pblv_start[]; \
	extern const char _binary_levels_ ## name ## _pblv_end[];   \
	const char *name = _binary_levels_ ## name ## _pblv_start

struct level {
	struct point start;
	struct block *blocks[HEIGHT_BLOCKS][WIDTH_BLOCKS];
};

struct level parse_level(char *level_string);

