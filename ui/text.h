#include "../native/common.h"
#include "../utils/position.h"

enum text_align {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

struct font {
	sprite_t sprite;
	int char_width;
	int char_height;
	int spacing;
};

extern struct font *font_5x7, *font_5x7_x2, *font_5x7_x4, *font_9x14;

void init_fonts();

void unload_fonts();

void draw_text(char *text, struct font *font, struct position position);

