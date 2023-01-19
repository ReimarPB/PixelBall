#include <stdbool.h>
#include "../globals.h"
#include "../native/common.h"
#include "scene.h"
#include "game.h"

float pause_brightness;
bool ending_pause = false;

void pause()
{
	pause_brightness = 1.0;
	scene = SCENE_PAUSE;
}

void update_pause()
{
	if (!ending_pause && pause_brightness > 0.6) {
		pause_brightness -= 0.05;
	}

	if (ending_pause) {
		if (pause_brightness < 1.0)
			pause_brightness += 0.1;
		else
			scene = SCENE_GAME;
	}

	brightness = pause_brightness;
}

void draw_pause(int x, int y, int width, int height)
{
	printf("%f\n", brightness);
	//brightness = pause_brightness;
	draw_game(0, 0, WIDTH_PX, HEIGHT_PX);
	//brightness = 1.0;
}

void pause_onkeydown(enum key key)
{
	if (key == KEY_ESCAPE) {
		ending_pause = true;
	}
}
