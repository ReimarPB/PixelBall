// TODO check platform
#include "x11.h"

void set_window_title(char *title);

void set_window_icon(sprite_t icon);

sprite_t load_sprite(sprite_identifier_t sprite);

void unload_sprite(sprite_t sprite);

void draw_sprite(sprite_t sprite, int x, int y);

void draw_rect(int x, int y, int width, int height, int red, int green, int blue);

void redraw_area(int x, int y, int width, int height);

