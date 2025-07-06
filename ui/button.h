#include <stdbool.h>

#include "../utils/position.h"
#include "../native/common.h"

#define MAX_BUTTON_AMOUNT 64

enum button_type {
	BUTTON_TYPE_STANDARD,
	BUTTON_TYPE_SMALL,
	BUTTON_TYPE_SQUARE,
};

struct button {
	char *text;
	struct position position;
	void (*callback)(void);
	enum button_type type;
};

extern struct button buttons[MAX_BUTTON_AMOUNT];

void init_buttons(void);

void draw_buttons(void);

void buttons_onkeydown(enum key key, bool ctrl, bool alt, bool shift);

void buttons_onkeyup(enum key key, bool ctrl, bool alt, bool shift);

void buttons_onmousemove(int x, int y);

void buttons_onmousedown(int mouse_btn, int x, int y);

void buttons_onmouseup(int mouse_btn, int x, int y);

void add_button(struct button button);

void clear_buttons(void);

