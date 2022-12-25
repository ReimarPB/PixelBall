#include "game.h"
#include "native/x11.h"
#include "native/common.h"

int ball_y;

void init()
{
	set_window_title("Pixel Ball");
	ball_y = 0;
}

void draw()
{
	sprite_t sprite_ball = load_sprite(SPRITE_BALL);
	draw_rect(0, 0, WIDTH_PX, HEIGHT_PX, 120, 200, 255);
	draw_sprite(sprite_ball, 12, ball_y);
}

void update()
{
	ball_y++;
	redraw_area(12, ball_y-1, BALL_SIZE, BALL_SIZE+1);
}

