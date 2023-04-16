#include <string.h>

#include "button.h"
#include "../native/common.h"
#include "../ui/text.h"

struct button buttons[MAX_BUTTON_AMOUNT];
int button_amount;

int get_button_width(enum button_type type)
{
	switch (type) {
		case BUTTON_TYPE_STANDARD: return 200;
	}
}

int get_button_height(enum button_type type)
{
	switch (type) {
		case BUTTON_TYPE_STANDARD: return 45;
	}
}

void clear_buttons()
{
	memset(&buttons, 0, sizeof(buttons));
	button_amount = 0;
}

void init_buttons()
{
	clear_buttons();
}

void draw_buttons(int x, int y, int width, int height)
{
	for (int i = 0; i < button_amount; i++) {
		struct button button = buttons[i];

		int width = get_button_width(button.type);
		int height = get_button_height(button.type);

		int btn_x = get_x_from_position(button.position, width);
		int btn_y = get_y_from_position(button.position, height);

		draw_rect(rgb(255, 0, 0), btn_x, btn_y, 200, 45);
		draw_rect(rgb(180, 0, 0), btn_x, btn_y + 45, 200, 5);

		draw_text(
			button.text,
			font_5x7_x2,
			pos(btn_x + width / 2, btn_y + height / 2, H_ALIGN_CENTER, V_ALIGN_MIDDLE)
		);
	}
}

void add_button(struct button button)
{
	buttons[button_amount++] = button;
}

