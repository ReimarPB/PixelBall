#include <stdbool.h>

#include "../globals.h"
#include "../native/common.h"
#include "../ui/text.h"
#include "../ui/button.h"
#include "../utils/position.h"
#include "scene.h"
#include "transition.h"
#include "game.h"

bool ending_pause = false;
float opacity;

void unpause(void)
{
	ending_pause = true;
}

void go_to_menu(void)
{
	transition_to_scene(SCENE_LEVEL_SELECT);
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

void pause(void)
{
	change_scene(SCENE_PAUSE);
	add_button(pause_buttons[0]);
	add_button(pause_buttons[1]);
	opacity = 0.0;
}

void update_pause(void)
{
	if (!ending_pause && opacity < 0.4) {
		opacity += 0.05;
		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}

	if (ending_pause) {
		if (opacity > 0.05) {
			opacity -= 0.05;
		} else {
			clear_buttons();
			change_scene(SCENE_GAME);
			ending_pause = false;
		}

		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}
}

void draw_pause(void)
{
	draw_game();

	draw_rect(rgba(0, 0, 0, opacity), 0, 0, WIDTH_PX, HEIGHT_PX);

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
