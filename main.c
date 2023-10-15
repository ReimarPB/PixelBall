#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "globals.h"
#include "main.h"
#include "scenes/scene.h"
#include "scenes/game.h"
#include "scenes/pause.h"
#include "scenes/main_menu.h"
#include "scenes/transition.h"
#include "entities/particle.h"
#include "entities/block.h"
#include "entities/ball.h"
#include "native/x11.h"
#include "native/common.h"
#include "ui/text.h"
#include "ui/button.h"

enum scene scene = SCENE_GAME;
sprite_t sprite_icon;

void init(void)
{
	srand(time(0));

	init_game();
	init_blocks();
	init_ball();
	init_fonts();
	init_buttons();

	set_window_title("Pixel Ball");

	sprite_icon = load_sprite(SPRITE_ICON);

	set_window_icon(sprite_icon);
}

void draw(void)
{
	switch (scene) {
		case SCENE_GAME:       draw_game();       break;
		case SCENE_PAUSE:      draw_pause();      break;
		case SCENE_MAIN_MENU:  draw_main_menu();  break;
		case SCENE_TRANSITION: draw_transition(); return; // don't draw buttons on top of transition
	}

	draw_buttons();
}

void update(void)
{
	switch (scene) {
		case SCENE_GAME:       update_game();       break;
		case SCENE_PAUSE:      update_pause();      break;
		case SCENE_TRANSITION: update_transition(); break;
	}
}

void onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (scene) {
		case SCENE_GAME:  game_onkeydown(key, ctrl, alt, shift);  break;
		case SCENE_PAUSE: pause_onkeydown(key, ctrl, alt, shift); break;
	}

	buttons_onkeydown(key, ctrl, alt, shift);
}

void onkeyup(enum key key, bool ctrl, bool alt, bool shift)
{
	game_onkeyup(key, ctrl, alt, shift);

	buttons_onkeyup(key, ctrl, alt, shift);
}

void onmousemove(int x, int y)
{
	buttons_onmousemove(x, y);
}

void onmousedown(int btn, int x, int y)
{
	buttons_onmousedown(btn, x, y);
}

void onmouseup(int btn, int x, int y)
{
	buttons_onmouseup(btn, x, y);
}

void unload(void)
{
	unload_sprite(sprite_icon);
	unload_blocks();
	unload_ball();
	unload_fonts();
}

