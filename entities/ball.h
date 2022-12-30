#include <stdbool.h>

#define BALL_SIZE 16
#define BALL_SPEED 0.2
#define BALL_MAX_SPEED 2.0

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

