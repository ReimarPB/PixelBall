#include "position.h"

int get_x_from_position(struct position pos, int width)
{
	switch (pos.h_align) {
		case H_ALIGN_LEFT:
			return pos.x;
		case H_ALIGN_CENTER:
			return pos.x - width / 2;
		case H_ALIGN_RIGHT:
			return pos.x - width;
	}
}

int get_y_from_position(struct position pos, int height)
{
	switch (pos.h_align) {
		case V_ALIGN_TOP:
			return pos.y;
		case V_ALIGN_MIDDLE:
			return pos.y - height / 2;
		case V_ALIGN_BOTTOM:
			return pos.y - height;
	}
}

struct position pos(int x, int y, enum h_align h_align, enum v_align v_align)
{
	return (struct position) POS(x, y, h_align, v_align);
}

