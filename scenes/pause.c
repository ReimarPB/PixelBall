#include <stdbool.h>
#include "../globals.h"
#include "../native/common.h"
#include "../ui/text.h"
#include "../ui/button.h"
#include "../utils/position.h"
#include "scene.h"
#include "game.h"

bool ending_pause = false;
float pause_brightness;

void unpause()
{
	ending_pause = true;
}

void go_to_menu()
{
	change_scene(SCENE_MAIN_MENU);
}

static struct button pause_buttons[] = {
	(struct button) {
		.text = "RESUME",
		.position = POS(WIDTH_PX / 2, 150, H_ALIGN_CENTER, V_ALIGN_TOP),
		.callback = unpause,
		.type = BUTTON_TYPE_STANDARD,
	},
	(struct button) {
		.text = "MAIN MENU",
		.position = POS(WIDTH_PX / 2, 220, H_ALIGN_CENTER, V_ALIGN_TOP),
		.callback = go_to_menu,
		.type = BUTTON_TYPE_STANDARD,
	},
};

void pause()
{
	change_scene(SCENE_PAUSE);
	add_button(pause_buttons[0]);
	add_button(pause_buttons[1]);
	pause_brightness = 1.0;
}

void update_pause()
{
	if (!ending_pause && pause_brightness > 0.6) {
		pause_brightness -= 0.05;
		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}

	if (ending_pause) {
		if (pause_brightness < 1.0) {
			pause_brightness += 0.1;
		} else {
			clear_buttons();
			change_scene(SCENE_GAME);
			ending_pause = false;
		}

		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}
}

void draw_pause(int x, int y, int width, int height)
{
	float old_brightness = brightness;
	brightness = pause_brightness;

	// Background
	draw_rect(
		rgb(129, 212, 250),
		x, y, width, height
	);


	draw_game(0, 0, WIDTH_PX, HEIGHT_PX);

	brightness = old_brightness;

	draw_text(
		"PAUSED",
		font_5x7_x4,
		pos(WIDTH_PX / 2, 50, H_ALIGN_CENTER, V_ALIGN_MIDDLE)
	);
}

void pause_onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	if (key == KEY_ESCAPE) {
		unpause();
	}
}
