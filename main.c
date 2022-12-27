#include "globals.h"
#include "scenes/game.h"
#include "native/x11.h"
#include "native/common.h"

void init()
{
	init_game();
	set_window_title("Pixel Ball");
}

void draw(int x, int y, int width, int height)
{
	draw_rect(x, y, width, height, 140, 220, 255);
	draw_game(x, y, width, height);
}

void update()
{
	update_game();
}

void onkeydown(enum key key)
{
	game_onkeydown(key);
}

void onkeyup(enum key key)
{
	game_onkeyup(key);
}

