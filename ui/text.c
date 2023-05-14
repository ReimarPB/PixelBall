#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../globals.h"
#include "text.h"
#include "../native/common.h"

const char *FONT_STRING = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct font *font_5x7, *font_5x7_x2, *font_5x7_x4, *font_9x14;

void init_fonts()
{
	font_5x7 = malloc(sizeof(struct font));
	*font_5x7 = (struct font) {
		.sprite = load_sprite(SPRITE_FONT_5x7),
		.char_width = 5,
		.char_height = 7,
		.spacing = 2,
	};

	font_5x7_x2 = malloc(sizeof(struct font));
	*font_5x7_x2 = (struct font) {
		.sprite = load_sprite(SPRITE_FONT_5x7_x2),
		.char_width = 10,
		.char_height = 14,
		.spacing = 4,
	};

	font_5x7_x4 = malloc(sizeof(struct font));
	*font_5x7_x4 = (struct font) {
		.sprite = load_sprite(SPRITE_FONT_5x7_x4),
		.char_width = 20,
		.char_height = 28,
		.spacing = 8,
	};
}

void unload_fonts()
{
	unload_sprite(font_5x7->sprite);
	unload_sprite(font_5x7_x2->sprite);
	unload_sprite(font_5x7_x4->sprite);
	free(font_5x7);
	free(font_5x7_x2);
	free(font_5x7_x4);
}

void draw_text(char *text, struct font *font, struct position position)
{
	int text_width = strlen(text) * font->char_width + (strlen(text) - 1) * font->spacing;
	int x = get_x_from_position(position, text_width);
	int y = get_y_from_position(position, font->char_height);

	for (int i = 0; i < strlen(text); i++) {
		int index = strchr(FONT_STRING, text[i]) - FONT_STRING; // TODO validate

		draw_partial_sprite(
			font->sprite,
			x + i * (font->char_width + font->spacing),
			y,
			index * font->char_width, 0,
			font->char_width, font->char_height
		);
	}
}

