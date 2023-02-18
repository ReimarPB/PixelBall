#include <stdbool.h>
#include "../globals.h"
#include "../native/common.h"
#include "../ui/text.h"
#include "scene.h"
#include "game.h"

bool ending_pause = false;

void pause()
{
	scene = SCENE_PAUSE;
}

void update_pause()
{
	if (!ending_pause && brightness > 0.6) {
		brightness -= 0.05;
		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}

	if (ending_pause) {
		if (brightness < 1.0) {
			brightness += 0.1;
		} else {
			scene = SCENE_GAME;
			ending_pause = false;
		}

		redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
	}
}

void draw_pause(int x, int y, int width, int height)
{
	draw_game(0, 0, WIDTH_PX, HEIGHT_PX);

	float old_brightness = brightness;
	brightness = 1.0;
	draw_text("PAUSED", font_white, 15, 15);
	brightness = old_brightness;
}

void pause_onkeydown(enum key key)
{
	if (key == KEY_ESCAPE) {
		ending_pause = true;
	}
}
