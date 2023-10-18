#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"

#define BLOCK_SHADOW_OFFSET 3

extern struct level level;

void init_game(void);

void update_game(void);

void draw_game(void);

void game_onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void game_onkeyup(enum key key, bool ctrl, bool alt, bool shift);

void die(void);

