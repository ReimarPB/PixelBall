#include "../globals.h"
#include "level.h"

struct level parse_level(char *level_string)
{
	struct level result = { 0 };

	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		int offset = y * (WIDTH_BLOCKS + 1);

		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			switch (level_string[offset + x]) {
				case 'o':
					result.start = (struct point) { .x = x, .y = y };
					break;
				case 'G':
					result.blocks[y][x] = &BLOCK_GRASS;
					break;
				case 'D':
					result.blocks[y][x] = &BLOCK_DIRT;
					break;
				case 'S':
					result.blocks[y][x] = &BLOCK_SPIKE;
					break;
			}
		}
	}

	return result;
}

