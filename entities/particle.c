#include <stdlib.h>

#include "particle.h"
#include "../native/common.h"

#define PARTICLE_SIZE 4
#define MAX_PARTICLES 64

struct particle {
	struct color color;
	float x;
	float y;
	float x_vel;
	float y_vel;
	float start_y;
	int active;
};

static struct particle particles[MAX_PARTICLES];

float random_float(float min, float max)
{
	return (max - min) * ((float)rand() / RAND_MAX) + min;
}

void clear_particles(void)
{
	for (int i = 0; i < MAX_PARTICLES; i++) {
		particles[i].active = 0;
	}
}

void init_particles(void)
{
	clear_particles();
}

void add_particle(struct color color, int x, int y, float min_x_vel, float max_x_vel, float min_y_vel, float max_y_vel)
{
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (!particles[i].active) {
			particles[i] = (struct particle) {
				.color = color,
				.x = x,
				.y = y,
				.x_vel = random_float(min_x_vel, max_x_vel),
				.y_vel = random_float(min_y_vel, max_y_vel),
				.start_y = y,
				.active = 1,
			};
			return;
		}
	}
}

void update_particles(void)
{
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			particles[i].x += particles[i].x_vel;
			particles[i].y += particles[i].y_vel;
			particles[i].y_vel += 0.02;

			// Delete particle when reaching below initial y coord
			if (particles[i].y > particles[i].start_y) {
				particles[i].active = 0;
				continue;
			}
		}
	}

	redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
}

void draw_particles(void)
{
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			draw_rect(particles[i].color, particles[i].x, particles[i].y, PARTICLE_SIZE, PARTICLE_SIZE);
		}
	}
}

