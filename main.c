#include <stdlib.h>
#include <time.h>

#include "globals.h"
#include "scenes/game.h"
#include "entities/particle.h"
#include "native/x11.h"
#include "native/common.h"

void init()
{
	srand(time(0));
	init_game();
	set_window_title("Pixel Ball");
}

void draw(int x, int y, int width, int height)
{
	draw_rect(
		(struct color) { .red = 140, .green = 220, .blue = 255 },
		x, y, width, height
	);
	draw_game(x, y, width, height);
	draw_particles(x, y, width, height);
}

void update()
{
	update_game();
	update_particles();
}

void onkeydown(enum key key)
{
	game_onkeydown(key);
}

void onkeyup(enum key key)
{
	game_onkeyup(key);
}

