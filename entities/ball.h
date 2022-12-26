#include <stdbool.h>

struct ball {
	float x;
	float y;
	float x_vel;
	float y_vel;
	bool going_left;
	bool going_right;
};

void update_ball(struct ball *ball);

void draw_ball(struct ball ball);

