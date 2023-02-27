#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "text.h"
#include "../native/common.h"

const char *FONT_STRING = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct font *font_5x7, *font_9x14;

void init_fonts()
{
	font_5x7 = malloc(sizeof(struct font));
	*font_5x7 = (struct font) {
		.sprite = load_sprite(SPRITE_FONT_5x7),
		.char_width = 5,
		.char_height = 7,
		.spacing = 2,
	};

	font_9x14 = malloc(sizeof(struct font));
	*font_9x14 = (struct font) {
		.sprite = load_sprite(SPRITE_FONT_9x14),
		.char_width = 9,
		.char_height = 14,
		.spacing = 2,
	};
}

void unload_fonts()
{
	unload_sprite(font_5x7->sprite);
	unload_sprite(font_9x14->sprite);
	free(font_5x7);
	free(font_9x14);
}

void draw_text(char *text, struct font *font, int x, int y)
{
	for (int i = 0; i < strlen(text); i++) {
		int index = strchr(FONT_STRING, text[i]) - FONT_STRING; // TODO validate
		draw_partial_sprite(
			font->sprite,
			x + i * font->char_width + (i-1) * font->spacing, y,
			index * font->char_width, 0,
			font->char_width, font->char_height
		);
	}
}

