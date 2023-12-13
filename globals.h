#ifndef GLOBALS_H
#define GLOBALS_H

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define FPS 60
#define BLOCK_SIZE 28
#define WIDTH_BLOCKS 30
#define HEIGHT_BLOCKS 15
#define WIDTH_PX WIDTH_BLOCKS * BLOCK_SIZE
#define HEIGHT_PX HEIGHT_BLOCKS * BLOCK_SIZE
#define BLOCK_SHADOW_OFFSET 3

enum axis {
	X_AXIS,
	Y_AXIS,
};

struct point {
	int x;
	int y;
};

struct rectangle {
	int x;
	int y;
	int width;
	int height;
};

#endif

