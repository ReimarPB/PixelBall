#include "globals.h"
#include "scenes/game.h"
#include "native/x11.h"
#include "native/common.h"

void init()
{
	init_game();
	set_window_title("Pixel Ball");
}

void draw()
{
	draw_rect(0, 0, WIDTH_PX, HEIGHT_PX, 140, 220, 255);
	draw_game();
}

void update()
{
	update_game();
}

