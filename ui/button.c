#include <string.h>

#include "button.h"
#include "../native/common.h"
#include "../ui/text.h"

struct button buttons[MAX_BUTTON_AMOUNT];
int button_amount;

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

		int x = button.x - 200 / 2;

		draw_rect(rgb(255, 0, 0), x, button.y, 200, 45);
		draw_rect(rgb(180, 0, 0), x, button.y + 45, 200, 5);

		draw_text(button.text, font_5x7_x2, ALIGN_CENTER, button.x, button.y + 15);
	}
}

void add_button(struct button button)
{
	buttons[button_amount++] = button;
}

