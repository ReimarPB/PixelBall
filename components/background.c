#include "../native/common.h"

void draw_background(int draw_x, int draw_y, int draw_width, int draw_height)
{
	draw_rect(rgb(129, 212, 250), draw_x, draw_y, draw_width, draw_height);

	for (int x = WIDTH_PX - 50, y = 200, v = 30; y < HEIGHT_PX; x -= v, y += 2, v = MAX(v / 1.01, 1)) {
		int start_x = MIN(x, draw_x);
		int start_y = MIN(y, draw_y);

		draw_rect(rgb(60, 143, 55), x, y, WIDTH_PX - x, 2);
	}
}

