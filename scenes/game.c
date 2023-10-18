#include <stddef.h>
#include <string.h>
#include <math.h>

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
sprite_t sprite_background;

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

void init_game(void)
{
	// TODO unload
	sprite_background = load_sprite(SPRITE_BACKGROUND);

	// Test level
	LOAD_LEVEL(test_level);

	level = parse_level(test_level);

	ball = (struct ball) {
		.x = level.start.x * BLOCK_SIZE,
		.y = level.start.y * BLOCK_SIZE,
		.x_vel = 0.0,
		.y_vel = 0.1
	};
}

void update_game(void)
{
	struct ball old_ball = ball;
	update_ball(&ball);
	redraw_area(keep_in_width_range(old_ball.x), keep_in_height_range(old_ball.y), BALL_SIZE, BALL_SIZE);
	redraw_area(keep_in_width_range(ball.x),     keep_in_height_range(ball.y),     BALL_SIZE, BALL_SIZE);
	update_particles();
}

void draw_game(void)
{
	draw_sprite(sprite_background, 0, 0);

	// Draw block shadows
	for (int y = 0; y < HEIGHT_BLOCKS; y++) {
		for (int x = 0; x < WIDTH_BLOCKS; x++) {
			if (level.blocks[y][x] == NULL) continue;

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

			draw_sprite(*level.blocks[y][x]->sprite, x * BLOCK_SIZE, y * BLOCK_SIZE);
		}
	}

	draw_ball(ball);
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

void die(void)
{
	for (int i = 0; i < 10; i++) {
		add_particle(rgb(255, 0, 0), ball.x + BALL_SIZE / 2, MIN(ball.y + BALL_SIZE / 2, HEIGHT_PX), -1.0,  1.0, -2.0, 2.0);
	}

	ball.x = level.start.x * BLOCK_SIZE;
	ball.y = level.start.y * BLOCK_SIZE;
	ball.x_vel = 0.0;
	ball.y_vel = 0.1;
}

