#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"

extern struct block *level[HEIGHT_BLOCKS][WIDTH_BLOCKS];

void init_game();

void update_game();

void draw_game(int x, int y, int width, int height);

void game_onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void game_onkeyup(enum key key, bool ctrl, bool alt, bool shift);

