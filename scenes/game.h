#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"

extern struct block *level[HEIGHT_BLOCKS][WIDTH_BLOCKS];

void init_game(void);

void update_game(void);

void draw_game(void);

void game_onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void game_onkeyup(enum key key, bool ctrl, bool alt, bool shift);

