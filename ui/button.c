#include <string.h>

#include "button.h"
#include "../globals.h"
#include "../native/common.h"
#include "../ui/text.h"

struct button buttons[MAX_BUTTON_AMOUNT];
int button_amount;
int focused_button = -1;

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
		int full_height = height + BUTTON_SHADOW_SIZE;

		int btn_x = get_x_from_position(button.position, width);
		int btn_y = get_y_from_position(button.position, height);

		draw_rect(rgb(255, 0, 0), btn_x, btn_y, width, height);
		draw_rect(rgb(180, 0, 0), btn_x, btn_y + height, width, BUTTON_SHADOW_SIZE);

		if (focused_button == i) {
			struct color border_color = rgb(255, 255, 255);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, btn_y - BUTTON_BORDER_SIZE, width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, btn_y + full_height,        width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, btn_y,                      BUTTON_BORDER_SIZE,             full_height);
			draw_rect(border_color, btn_x + width,              btn_y,                      BUTTON_BORDER_SIZE,             full_height);
		}

		draw_text(
			button.text,
			font_5x7_x2,
			pos(btn_x + width / 2, btn_y + height / 2, H_ALIGN_CENTER, V_ALIGN_MIDDLE)
		);
	}
}

void buttons_onkeydown(enum key key)
{
	switch (key) {
		case KEY_TAB:

			focused_button++;
			if (focused_button >= button_amount)
				focused_button = -1;

			redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);

			break;
		case KEY_ENTER:

			if (focused_button >= 0) {
				buttons[focused_button].callback();
				focused_button = -1;
			}

			break;
		default:
			break;

	}
}

void update_buttons()
{

}

void add_button(struct button button)
{
	buttons[button_amount++] = button;
}

