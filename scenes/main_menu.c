#include <stdlib.h>
#include <string.h>

#include "../globals.h"
#include "../native/common.h"
#include "../ui/button.h"
#include "../entities/block.h"
#include "../scenes/game.h"
#include "../levels/level.h"
#include "transition.h"

#define INITIAL_BACKGROUND_HEIGHT 4
#define BACKGROUND_HEIGHT_SPAN 2

static sprite_t sprite_logo, sprite_background;

int background_heights[WIDTH_BLOCKS + 1];
int background_offset = 0;

void play(void)
{
	transition_to_scene(SCENE_LEVEL_SELECT);
}

static struct button play_button = {
	.text = "PLAY",
	.position = POS(WIDTH_PX / 2, 250, H_ALIGN_CENTER, V_ALIGN_TOP),
	.callback = play,
	.type = BUTTON_TYPE_STANDARD,
	.disabled = false,
};

int generate_background_height(int height)
{
	int random = rand() % 8;

	if (random == 1)
		height += height < INITIAL_BACKGROUND_HEIGHT + BACKGROUND_HEIGHT_SPAN ? 1 : -1;
	else if (random == 2)
		height += height > INITIAL_BACKGROUND_HEIGHT - BACKGROUND_HEIGHT_SPAN ? -1 : 1;

	return height;
}

void init_main_menu(void)
{
	sprite_logo = load_sprite(SPRITE_LOGO);
	sprite_background = load_sprite(SPRITE_BACKGROUND);
}

void load_main_menu()
{
	// Generate block heights for background
	int height = INITIAL_BACKGROUND_HEIGHT;
	for (int i = 0; i <= WIDTH_BLOCKS; i++) {
		height = generate_background_height(height);
		background_heights[i] = height;
	}

	add_button(play_button);
}

void update_main_menu(void)
{
	background_offset++;
	background_offset %= BLOCK_SIZE;

	if (background_offset == 0) {
		for (int i = WIDTH_BLOCKS; i > 0; i--) {
			background_heights[i] = background_heights[i-1];
		}

		background_heights[0] = generate_background_height(background_heights[1]);
	}

	redraw();
}

void draw_main_menu(void)
{
	draw_sprite(sprite_background, 0, 0);

	// Draw menu background
	for (int i = 0; i <= WIDTH_BLOCKS; i++) {
		int block_x = (i - 1) * BLOCK_SIZE + background_offset;

		// Draw grass
		draw_sprite(*BLOCK_GRASS.sprite, block_x, HEIGHT_PX - BLOCK_SIZE * background_heights[i]);

		// Draw dirt
		for (int j = 0; j < background_heights[i]; j++) {
			sprite_t *sprite = BLOCK_DIRT.sprite;
			if (j < background_heights[i] / 2) sprite = BLOCK_ROCK.sprite;

			draw_sprite(*sprite, block_x, HEIGHT_PX - BLOCK_SIZE * j);
		}

		// Draw shadow
		if (i > 0 && background_heights[i-1] > background_heights[i]) {
			draw_rect(
				rgba(0, 0, 0, 0.3),
				block_x,
				HEIGHT_PX - background_heights[i-1] * BLOCK_SIZE + BLOCK_SHADOW_OFFSET,
				BLOCK_SHADOW_OFFSET,
				BLOCK_SIZE * (background_heights[i-1] - background_heights[i]) - BLOCK_SHADOW_OFFSET
			);
		}
	}

	draw_sprite(sprite_logo, WIDTH_PX / 2 - get_sprite_width(sprite_logo) / 2, 50);
}

void unload_main_menu(void)
{
	unload_sprite(sprite_logo);
	unload_sprite(sprite_background);
}

