#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "../entities/ball.h"
#include "../entities/block.h"
#include "../entities/particle.h"
#include "../scenes/pause.h"
#include "../native/common.h"
#include "../components/background.h"
#include "../levels/level.h"
#include "../globals.h"
#include "game.h"

static struct ball ball = { 0 };
struct level level;
static void *block_states[HEIGHT_PX][WIDTH_PX] = { 0 };
static sprite_t sprite_background;

int ticks;

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

void reset_block_states()
{
	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			if (!level.blocks[y][x] || level.blocks[y][x]->default_state_size == 0) continue;

			if (block_states[y][x]) free(block_states[y][x]);

			block_states[y][x] = malloc(level.blocks[y][x]->default_state_size);
			memcpy(block_states[y][x], level.blocks[y][x]->default_state, level.blocks[y][x]->default_state_size);
		}
	}
}

void init_game(void)
{
	sprite_background = load_sprite(SPRITE_BACKGROUND);
}

void start_game(const char *lvl)
{
	level = parse_level(lvl);
	reset_block_states();

	ball = (struct ball) {
		.x = level.start.x * BLOCK_SIZE,
		.y = level.start.y * BLOCK_SIZE,
		.x_vel = 0.0,
		.y_vel = 0.1
	};

	ticks = 0;
}

void update_game(void)
{
	struct ball old_ball = ball;
	update_ball(&ball, block_states);
	redraw_area(keep_in_width_range(old_ball.x), keep_in_height_range(old_ball.y), BALL_SIZE, BALL_SIZE);
	redraw_area(keep_in_width_range(ball.x),     keep_in_height_range(ball.y),     BALL_SIZE, BALL_SIZE);
	update_particles();

	ticks++;
}

void draw_game(void)
{
	draw_sprite(sprite_background, 0, 0);

	// Draw block shadows
	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			if (level.blocks[y][x] == NULL || !level.blocks[y][x]->has_shadow) continue;

			draw_rect(
				rgba(0, 0, 0, 0.3),
				x * BLOCK_SIZE + (x ? BLOCK_SHADOW_OFFSET : 0),
				y * BLOCK_SIZE + (y ? BLOCK_SHADOW_OFFSET : 0),
				x ? BLOCK_SIZE : BLOCK_SIZE + BLOCK_SHADOW_OFFSET,
				y ? BLOCK_SIZE : BLOCK_SIZE + BLOCK_SHADOW_OFFSET
			);
		}
	}

	// Draw blocks
	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			if (level.blocks[y][x] == NULL) continue;

			if (level.blocks[y][x]->draw_function)
				level.blocks[y][x]->draw_function(x, y, block_states[y][x]);
			else
				draw_sprite(*level.blocks[y][x]->sprite, x * BLOCK_SIZE, y * BLOCK_SIZE);
		}
	}


	// Draw hitboxes (debug)
	if (getenv("PIXELBALL_DEBUG")) {
		for (int y = 0; y < HEIGHT_BLOCKS; y++) {
			for (int x = 0; x < WIDTH_BLOCKS; x++) {
				struct block *block = level.blocks[y][x];

				if (!block || !block->hitbox) continue;

				draw_rect(
					rgba(0, 0, 255, 0.3),
					x * BLOCK_SIZE + block->hitbox->x,
					y * BLOCK_SIZE + block->hitbox->y,
					block->hitbox->width,
					block->hitbox->height
				);
			}
		}
	}

	draw_ball(ball);

	// Draw ball hitbox (debug)
	if (getenv("PIXELBALL_DEBUG")) {
		draw_rect(rgba(0, 0, 255, 0.3), ball.x, ball.y, BALL_SIZE, BALL_SIZE);
	}

	draw_particles();
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

void unload_game(void)
{
	unload_sprite(sprite_background);
}

void die(void)
{
	for (int i = 0; i < 10; i++) {
		add_particle(rgb(255, 0, 0), ball.x + BALL_SIZE / 2, MIN(ball.y + BALL_SIZE / 2, HEIGHT_PX), -1.0, 1.0, -2.0, 2.0);
	}

	ball.x = level.start.x * BLOCK_SIZE;
	ball.y = level.start.y * BLOCK_SIZE;
	ball.x_vel = 0.0;
	ball.y_vel = 0.1;

	reset_block_states();
}

