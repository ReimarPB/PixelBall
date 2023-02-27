#include "../native/common.h"

struct font {
	sprite_t sprite;
	int char_width;
	int char_height;
	int spacing;
};

extern struct font *font_5x7, *font_9x14;

void init_fonts();

void unload_fonts();

void draw_text(char *text, struct font *font, int x, int y);

