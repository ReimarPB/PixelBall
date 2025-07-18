#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../globals.h"
#include "../scenes/scene.h"
#include "../native/common.h"
#include "button.h"
#include "text.h"

#define BUTTON_BORDER_SIZE 2
#define BUTTON_SHADOW_SIZE 5

struct button buttons[MAX_BUTTON_AMOUNT];
int button_amount, focused_button;
bool button_pressed;

int get_button_width(enum button_type type)
{
	switch (type) {
		case BUTTON_TYPE_STANDARD: return 200;
		case BUTTON_TYPE_SMALL:    return 70;
		case BUTTON_TYPE_SQUARE:   return 60;
	}
}

int get_button_height(enum button_type type)
{
	switch (type) {
		case BUTTON_TYPE_STANDARD: return 45;
		case BUTTON_TYPE_SMALL:    return 30;
		case BUTTON_TYPE_SQUARE:   return 60;
	}
}

bool button_collides_with(struct button button, int x, int y)
{
	int width = get_button_width(button.type);
	int height = get_button_height(button.type) + BUTTON_SHADOW_SIZE;

	int btn_x = get_x_from_position(button.position, width);
	int btn_y = get_y_from_position(button.position, height);

	return
		x >= btn_x && x <= btn_x + width &&
		y >= btn_y && y <= btn_y + height;
}

// Returns the index or -1 - ignores disabled buttons
int get_button_from_coords(int x, int y)
{
	for (int i = 0; i < button_amount; i++) {
		struct button button = buttons[i];

		if (button.disabled) continue;

		if (button_collides_with(button, x, y)) return i;
	}

	return -1;
}

void clear_buttons(void)
{
	memset(&buttons, 0, sizeof(buttons));
	button_amount = 0;
	focused_button = -1;
	button_pressed = false;
}

void init_buttons(void)
{
	clear_buttons();
}

void draw_buttons(void)
{
	for (int i = 0; i < button_amount; i++) {
		struct button button = buttons[i];

		int btn_width = get_button_width(button.type);
		int btn_height = get_button_height(button.type);
		int full_height = btn_height + BUTTON_SHADOW_SIZE;

		int btn_x = get_x_from_position(button.position, btn_width);
		int btn_y = get_y_from_position(button.position, btn_height);
		int y_offset = focused_button == i && button_pressed ? BUTTON_SHADOW_SIZE : 0;

		struct color color = button.disabled ? rgb(150, 150, 150) : rgb(255, 0, 0);
		struct color shadow_color = button.disabled ? rgb(120, 120, 120) : rgb(180, 0, 0);

		// Draw button
		if (focused_button == i && button_pressed) {
			draw_rect(color, btn_x, y_offset + btn_y, btn_width, btn_height);
		} else {
			draw_rect(color, btn_x, btn_y, btn_width, btn_height);
			draw_rect(shadow_color, btn_x, btn_y + btn_height, btn_width, BUTTON_SHADOW_SIZE);
		}

		// Draw border
		if (focused_button == i) {
			struct color border_color = rgb(255, 255, 255);

			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, y_offset + btn_y - BUTTON_BORDER_SIZE, btn_width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, btn_y + full_height,                   btn_width + BUTTON_BORDER_SIZE * 2, BUTTON_BORDER_SIZE);
			draw_rect(border_color, btn_x - BUTTON_BORDER_SIZE, y_offset + btn_y,                      BUTTON_BORDER_SIZE,                 full_height - y_offset);
			draw_rect(border_color, btn_x + btn_width,          y_offset + btn_y,                      BUTTON_BORDER_SIZE,                 full_height - y_offset);
		}

		draw_text(
			button.text,
			font_5x7_x2,
			pos(btn_x + btn_width / 2, y_offset + btn_y + btn_height / 2, H_ALIGN_CENTER, V_ALIGN_MIDDLE)
		);
	}
}

void click_button(int index)
{
	if (scene == SCENE_TRANSITION) return;

	buttons[index].callback();
}

void buttons_onkeydown(enum key key, bool ctrl, bool alt, bool shift)
{
	switch (key) {
		case KEY_TAB:
			if (shift) {
				focused_button--;
				if (focused_button < -1) focused_button = button_amount - 1;
			} else {
				focused_button++;
				if (focused_button >= button_amount) focused_button = -1;
			}

			redraw();
			break;
		case KEY_ENTER:
		case KEY_SPACE:

			if (focused_button >= 0) {
				button_pressed = true;
				redraw();
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
				click_button(focused_button);

			button_pressed = false;

			redraw();
			break;
		default:
			break;
	}
}

void buttons_onmousemove(int x, int y)
{
	int old_focused_button = focused_button;

	focused_button = get_button_from_coords(x, y);

	if (old_focused_button != focused_button)
		redraw();
}

void buttons_onmousedown(int mouse_btn, int x, int y)
{
	if (mouse_btn != 1) return;

	if (get_button_from_coords(x, y) == focused_button && focused_button >= 0) {
		button_pressed = true;

		redraw();
	}
}

void buttons_onmouseup(int mouse_btn, int x, int y)
{
	if (mouse_btn != 1) return;

	if (get_button_from_coords(x, y) == focused_button && focused_button >= 0 && button_pressed) {
		click_button(focused_button);

		redraw();
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

