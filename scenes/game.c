#include <stddef.h>
#include <string.h>
#include <math.h>

#include "../entities/ball.h"
#include "../entities/block.h"
#include "../entities/particle.h"
#include "../scenes/pause.h"
#include "../native/common.h"
#include "../globals.h"
#include "game.h"

static struct ball ball = { 0 };
struct block *level[HEIGHT_BLOCKS][WIDTH_BLOCKS] = { 0 };

int keep_in_width_range(float x)
{
	if (x < 0) return 0;
	if (x > WIDTH_PX) return WIDTH_PX;
	return x;
}

int keep_in_height_range(float x)
{
	if (x < 0) return 0;
	if (x > HEIGHT_PX) return HEIGHT_PX;
	return x;
}

void init_game()
{
	ball = (struct ball){
		.x = 2.0,
		.y = 0.0,
		.x_vel = 0.0,
		.y_vel = 0.1
	};

	// Test level
	level[8][0] = &BLOCK_GRASS;
	level[8][1] = &BLOCK_GRASS;
	level[8][2] = &BLOCK_GRASS;
	level[8][4] = &BLOCK_GRASS;
	level[8][5] = &BLOCK_GRASS;
	level[8][6] = &BLOCK_GRASS;
	level[8][7] = &BLOCK_GRASS;
	level[7][8] = &BLOCK_GRASS;
	level[7][9] = &BLOCK_GRASS;
	level[6][10] = &BLOCK_GRASS;
	level[6][11] = &BLOCK_GRASS;
	level[5][8] = &BLOCK_GRASS;
}

void update_game()
{
	struct ball old_ball = ball;
	update_ball(&ball);
	redraw_area(keep_in_width_range(old_ball.x), keep_in_height_range(old_ball.y), BALL_SIZE, BALL_SIZE);
	redraw_area(keep_in_width_range(ball.x),     keep_in_height_range(ball.y),     BALL_SIZE, BALL_SIZE);
	update_particles();
}

void draw_game(int x, int y, int width, int height)
{
	// Background
	draw_rect(
		rgb(129, 212, 250),
		x, y, width, height
	);

	int min_block_x = floor(x / BLOCK_SIZE);
	int max_block_x = ceil((x + width) / BLOCK_SIZE);
	int min_block_y = floor(y / BLOCK_SIZE);
	int max_block_y = ceil((y + height) / BLOCK_SIZE);

	// Keep within bounds
	if (max_block_x >= WIDTH_BLOCKS)  max_block_x = WIDTH_BLOCKS - 1;
	if (max_block_y >= HEIGHT_BLOCKS) max_block_y = HEIGHT_BLOCKS - 1;

	for (int y = min_block_y; y <= max_block_y; y++) {
		for (int x = min_block_x; x <= max_block_x; x++) {
			if (level[y][x] == NULL) continue;

			// TODO only load sprites once
			sprite_t sprite = load_sprite(level[y][x]->sprite);
			draw_sprite(sprite, x * BLOCK_SIZE, y * BLOCK_SIZE);
			unload_sprite(sprite);
		}
	}

	draw_ball(ball);
	draw_particles(x, y, width, height);
}

void game_onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_ARROW_LEFT:
			ball.going_left = true;
			break;
		case KEY_ARROW_RIGHT:
			ball.going_right = true;
			break;
		case KEY_ESCAPE:
			pause();
			break;
	}
}

void game_onkeyup(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_ARROW_LEFT:
			ball.going_left = false;
			break;
		case KEY_ARROW_RIGHT:
			ball.going_right = false;
			break;
	}
}

