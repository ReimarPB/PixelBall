#include <stdbool.h>

#include "native/common.h"

void init(void);

void draw(void);

void update(void);

void onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void onkeyup(enum key key, bool ctrl, bool alt, bool shift);

void onmousemove(int x, int y);

void onmousedown(int btn, int x, int y);

void onmouseup(int btn, int x, int y);

void unload(void);

