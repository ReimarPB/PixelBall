#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../native/common.h"
#include "../ui/button.h"
#include "../ui/text.h"
#include "../utils/position.h"
#include "../scenes/scene.h"
#include "../scenes/transition.h"
#include "../scenes/game.h"
#include "../levels/level.h"

#define LEVEL_AMOUNT 21

static struct button level_buttons[LEVEL_AMOUNT];

static char *level_button_labels[LEVEL_AMOUNT];
static sprite_t sprite_background;

LOAD_LEVEL(test_level);

void back(void)
{
	transition_to_scene(SCENE_MAIN_MENU);
}

static struct button back_button = {
	.text = "BACK",
	.position = POS(45, 25, H_ALIGN_CENTER, V_ALIGN_MIDDLE),
	.callback = back,
	.type = BUTTON_TYPE_SMALL,
	.disabled = false,
};

void play_level(void)
{
	start_game(test_level);
	transition_to_scene(SCENE_GAME);
}

void init_level_select(void)
{
	sprite_background = load_sprite(SPRITE_BACKGROUND);
}

void load_level_select(void)
{
	add_button(back_button);

	for (int i = 0; i < LEVEL_AMOUNT; i++) {
		level_button_labels[i] = malloc(3);
		sprintf(level_button_labels[i], "%d", i + 1);

		level_buttons[i] = (struct button) {
			.text = level_button_labels[i],
			.position = pos(
				i % 7 * 90 + 140,
				((int)i / 7) * 100 + 120,
				H_ALIGN_CENTER,
				V_ALIGN_MIDDLE
			),
			.callback = &play_level,
			.type = BUTTON_TYPE_SQUARE,
			.disabled = i != 0,
		};

		add_button(level_buttons[i]);
	}
}

void draw_level_select(void)
{
	draw_sprite(sprite_background, 0, 0);

	draw_rect(rgba(0, 0, 0, 0.5), 0, 0, WIDTH_PX, 50);
	draw_rect(rgba(0, 0, 0, 0.2), 0, 50, WIDTH_PX, HEIGHT_PX - 50);

	draw_text("SELECT LEVEL", font_5x7_x2, pos(WIDTH_PX / 2, 25, H_ALIGN_CENTER, V_ALIGN_MIDDLE));
}

void level_select_onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_ESCAPE:
			transition_to_scene(SCENE_MAIN_MENU);
			break;
	}
}

void unload_level_select(void)
{
	unload_sprite(sprite_background);
}

