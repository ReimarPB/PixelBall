#define MAX_BUTTON_AMOUNT 64

enum button_type {
	BUTTON_TYPE_STANDARD,
};

struct button {
	char *text;
	int x;
	int y;
	void (*callback)(void);
	enum button_type type;
};

extern struct button buttons[MAX_BUTTON_AMOUNT];

void init_buttons();

void draw_buttons(int x, int y, int width, int height);

void add_button(struct button button);

void clear_buttons();

