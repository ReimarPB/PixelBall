#ifndef NATIVE_COMMON_H
#define NATIVE_COMMON_H

#include <stdint.h>

// TODO check platform
#include "x11.h"
#include "../globals.h"

enum key {
	KEY_UNKNOWN,
	KEY_ARROW_LEFT,
	KEY_ARROW_RIGHT,
	KEY_ARROW_UP,
	KEY_ARROW_DOWN,
	KEY_ESCAPE,
	KEY_TAB,
	KEY_ENTER,
	KEY_SPACE,
};

struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	float alpha;
};

extern float brightness;

struct color rgb(uint8_t red, uint8_t green, uint8_t blue);

struct color rgba(uint8_t red, uint8_t green, uint8_t blue, float alpha);

void apply_brightness(struct color *color, float brightness);

void set_window_title(char *title);

void set_window_icon(sprite_t icon);

int get_sprite_width(sprite_t sprite);

int get_sprite_height(sprite_t sprite);

sprite_t load_sprite(sprite_identifier_t sprite);

void unload_sprite(sprite_t sprite);

void draw_sprite(sprite_t sprite, int x, int y);

void draw_partial_sprite(sprite_t sprite, int x, int y, int sprite_x, int sprite_y, int sprite_width, int sprite_height);

void draw_rect(struct color color, int x, int y, int width, int height);

void redraw(void);

void redraw_area(int x, int y, int width, int height);

struct point get_mouse_coords(void);

#endif

