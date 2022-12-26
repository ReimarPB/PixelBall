#include <stddef.h>
#include <string.h>

#include "../entities/ball.h"
#include "../entities/block.h"
#include "../native/common.h"
#include "../globals.h"
#include "game.h"

static struct ball ball = { 0 };
struct block *level[HEIGHT_BLOCKS][WIDTH_BLOCKS] = { 0 };

void init_game()
{
	ball = (struct ball){
		.x = 2.0,
		.y = 0.0,
		.x_vel = 0.0,
		.y_vel = 0.1
	};
}

void update_game()
{
	level[8][0] = &BLOCK_GRASS;
	level[8][1] = &BLOCK_GRASS;
	level[8][2] = &BLOCK_GRASS;
	level[8][3] = &BLOCK_GRASS;
	level[8][4] = &BLOCK_GRASS;
	level[8][5] = &BLOCK_GRASS;
	level[8][6] = &BLOCK_GRASS;
	level[8][7] = &BLOCK_GRASS;
	level[7][3] = &BLOCK_GRASS;

	for (int y = 0; y < sizeof(level) / sizeof(struct block*[WIDTH_BLOCKS]); y++) {
		for (int x = 0; x < sizeof(level[y]) / sizeof(struct block); x++) {
			if (level[y][x] == NULL) continue;

			level[y][x]->check_collision(&ball, x * BLOCK_SIZE, y * BLOCK_SIZE);
		}
	}

	update_ball(&ball);
	redraw_area(0, 0, 1, 1); // TODO
}

void draw_game()
{
	for (int y = 0; y < sizeof(level) / sizeof(struct block*[WIDTH_BLOCKS]); y++) {
		for (int x = 0; x < sizeof(level[y]) / sizeof(struct block); x++) {
			if (level[y][x] == NULL) continue;

			sprite_t sprite = load_sprite(level[y][x]->sprite);
			draw_sprite(sprite, x * BLOCK_SIZE, y * BLOCK_SIZE);
			unload_sprite(sprite);
		}
	}

	draw_ball(ball);
}

void game_onkeydown(enum key key)
{
	switch (key) {
		case KEY_ARROW_LEFT:
			ball.going_left = true;
			break;
		case KEY_ARROW_RIGHT:
			ball.going_right = true;
			break;
	}
}

void game_onkeyup(enum key key)
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

