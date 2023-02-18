#include <stdlib.h>
#include <time.h>

#include "globals.h"
#include "scenes/scene.h"
#include "scenes/game.h"
#include "scenes/pause.h"
#include "entities/particle.h"
#include "native/x11.h"
#include "native/common.h"

enum scene scene = SCENE_GAME;

void init()
{
	srand(time(0));
	init_game();
	set_window_title("Pixel Ball");

	sprite_t icon = load_sprite(SPRITE_ICON);
	set_window_icon(icon);
}

void draw(int x, int y, int width, int height)
{
	draw_rect(
		rgb(140, 220, 255),
		x, y, width, height
	);

	switch (scene) {
		case SCENE_GAME:  draw_game(x, y, width, height);  break;
		case SCENE_PAUSE: draw_pause(x, y, width, height); break;
	}
}

void update()
{
	switch (scene) {
		case SCENE_GAME:  update_game();  break;
		case SCENE_PAUSE: update_pause(); break;
	}
}

void onkeydown(enum key key)
{
	switch (scene) {
		case SCENE_GAME:  game_onkeydown(key);  break;
		case SCENE_PAUSE: pause_onkeydown(key); break;
	}
}

void onkeyup(enum key key)
{
	game_onkeyup(key);
}

