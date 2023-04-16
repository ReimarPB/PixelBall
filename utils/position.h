#ifndef POSITION_H
#define POSITION_H

#define POS(X, Y, H_ALIGN, V_ALIGN) { \
	.x = X, \
	.y = Y, \
	.h_align = H_ALIGN, \
	.v_align = V_ALIGN, \
}

enum v_align {
	V_ALIGN_TOP,
	V_ALIGN_MIDDLE,
	V_ALIGN_BOTTOM,
};

enum h_align {
	H_ALIGN_LEFT,
	H_ALIGN_CENTER,
	H_ALIGN_RIGHT,
};

struct position {
	int x;
	int y;
	enum h_align h_align;
	enum v_align v_align;
};

int get_x_from_position(struct position pos, int width);

int get_y_from_position(struct position pos, int height);

struct position pos(int x, int y, enum h_align h_align, enum v_align v_align);

#endif

