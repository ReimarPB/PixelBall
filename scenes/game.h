#include <stdbool.h>

#include "../native/common.h"
#include "../globals.h"

extern struct level level;

extern int ticks;

void init_game(void);

void start_game(const char *level);

void update_game(void);

void draw_game(void);

void game_onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void game_onkeyup(enum key key, bool ctrl, bool alt, bool shift);

void unload_game(void);

void die(void);

