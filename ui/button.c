#include <string.h>
#include <stdbool.h>

#include "button.h"
#include "../globals.h"
#include "../native/common.h"
#include "../ui/text.h"

#define BUTTON_BORDER_SIZE 2
#define BUTTON_SHADOW_SIZE 5

struct button buttons[MAX_BUTTON_AMOUNT];
int button_amount, focused_button;
bool button_pressed;

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

bool button_collides_with(struct button button, int x, int y)
{
	int width = get_button_width(button.type);
	int height = get_button_height(button.type);

	int btn_x = get_x_from_position(button.position, width);
	int btn_y = get_y_from_position(button.position, height);

	return
		x >= btn_x && x <= btn_x + width &&
		y >= btn_y && y <= btn_y + height;
}

// Returns the index or -1
int get_button_from_coords(int x, int y)
{
	for (int i = 0; i < button_amount; i++) {
		struct button button = buttons[i];

		if (button_collides_with(button, x, y)) return i;
	}

	return -1;
}

void redraw_button(int button_index)
{
	if (button_index < 0) return;

	struct button button = buttons[button_index];

	int width = get_button_width(button.type);
	int height = get_button_height(button.type);

	int x = get_x_from_position(button.position, width) - BUTTON_BORDER_SIZE;
	int y = get_y_from_position(button.position, height) - BUTTON_BORDER_SIZE;

	width += BUTTON_BORDER_SIZE * 2;
	height += BUTTON_BORDER_SIZE * 2;

	redraw_area(x, y, width, height);
}

void clear_buttons()
{
	memset(&buttons, 0, sizeof(buttons));
	button_amount = 0;
	focused_button = -1;
	button_pressed = false;
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
		int y_offset = focused_button == i && button_pressed ? BUTTON_SHADOW_SIZE : 0;

		// Draw button
		if (focused_button == i && button_pressed) {
			draw_rect(rgb(255, 0, 0), btn_x, y_offset + btn_y, width, height);
		} else {
			draw_rect(rgb(255, 0, 0), btn_x, btn_y, width, height);
			draw_rect(rgb(180, 0, 0), btn_x, btn_y + height, width, BUTTON_SHADOW_SIZE);
		}

		// Draw border
		if (focused_button == i) {
			struct color border_color = rgb(255, 255, 255);

			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, y_offset + btn_y - BUTTON_BORDER_SIZE, width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, btn_y + full_height,        width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, y_offset + btn_y,                      BUTTON_BORDER_SIZE,             full_height - y_offset);
			draw_rect(border_color, btn_x + width,              y_offset + btn_y,                      BUTTON_BORDER_SIZE,             full_height - y_offset);
		}

		draw_text(
			button.text,
			font_5x7_x2,
			pos(btn_x + width / 2, y_offset + btn_y + height / 2, H_ALIGN_CENTER, V_ALIGN_MIDDLE)
		);
	}
}

void buttons_onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_TAB:
			redraw_button(focused_button);

			if (shift) {
				focused_button--;
				if (focused_button < -1) focused_button = button_amount - 1;
			} else {
				focused_button++;
				if (focused_button >= button_amount) focused_button = -1;
			}

			redraw_button(focused_button);
			break;
		case KEY_ENTER:
		case KEY_SPACE:

			if (focused_button >= 0) {
				button_pressed = true;
				redraw_button(focused_button);
			}

			break;
		default:
			break;

	}
}

void buttons_onkeyup(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_ENTER:
		case KEY_SPACE:

			if (focused_button >= 0)
				buttons[focused_button].callback();

			button_pressed = false;
			redraw_button(focused_button);

			break;
		default:
			break;
	}
}

void buttons_onmousemove(int x, int y)
{
	int old_focused_button = focused_button;

	focused_button = get_button_from_coords(x, y);

	redraw_button(old_focused_button);
	redraw_button(focused_button);
}

void buttons_onmousedown(int mouse_btn, int x, int y)
{
	if (mouse_btn != 1) return;

	if (get_button_from_coords(x, y) == focused_button && focused_button >= 0) {
		button_pressed = true;

		redraw_button(focused_button);
	}
}

void buttons_onmouseup(int mouse_btn, int x, int y)
{
	if (mouse_btn != 1) return;

	if (get_button_from_coords(x, y) == focused_button && focused_button >= 0 && button_pressed) {
		buttons[focused_button].callback();

		redraw_button(focused_button);
	}

	button_pressed = false;
}

void add_button(struct button button)
{
	buttons[button_amount++] = button;

	// Check if mouse is already over button
	struct point mouse_coords = get_mouse_coords();

	if (button_collides_with(button, mouse_coords.x, mouse_coords.y))
		focused_button = button_amount - 1;
}

