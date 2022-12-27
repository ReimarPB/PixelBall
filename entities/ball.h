#include <stdbool.h>

#define BALL_SIZE 10
#define BALL_SPEED 0.1
#define BALL_MAX_SPEED 1.0

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

