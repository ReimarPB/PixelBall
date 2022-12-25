#include "game.h"
#include "native/x11.h"
#include "native/common.h"

void init()
{
	set_window_title("Pixel Ball");
}

void draw()
{
	sprite_t sprite_ball = load_sprite(SPRITE_BALL);
	draw_rect(0, 0, WIDTH_PX, HEIGHT_PX, 120, 200, 255);
	draw_sprite(sprite_ball, 2, 2);
}

void update()
{
}

