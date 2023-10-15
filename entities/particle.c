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
	struct particle *next;
};

struct particle *particle_head = NULL;

float random_float(float min, float max)
{
	return (max - min) * ((float)rand() / RAND_MAX) + min;
}

void add_particle(struct color color, int x, int y, float min_x_vel, float max_x_vel, float min_y_vel, float max_y_vel)
{
	struct particle *particle = malloc(sizeof(struct particle));

	*particle = (struct particle) {
		.color = color,
		.x = x,
		.y = y,
		.x_vel = random_float(min_x_vel, max_x_vel),
		.y_vel = random_float(min_y_vel, max_y_vel),
		.start_y = y,
	};

	particle->next = particle_head;
	particle_head = particle;
}

void update_particles()
{
	for (struct particle *particle = particle_head, *prev = NULL; particle != NULL; particle = particle->next) {
		redraw_area(particle->x, particle->y, PARTICLE_SIZE, PARTICLE_SIZE);

		particle->x += particle->x_vel;
		particle->y += particle->y_vel;
		particle->y_vel += 0.02;

		// Delete particle when reaching below inital y coord
		if (particle->y > particle->start_y) {
			if (prev) prev->next = particle->next;
			else particle_head = particle->next;
			free(particle);
			continue;
		}

		redraw_area(particle->x, particle->y, PARTICLE_SIZE, PARTICLE_SIZE);

		prev = particle;
	}
}

void draw_particles()
{
	for (struct particle *particle = particle_head; particle != NULL; particle = particle->next) {
		draw_rect(particle->color, particle->x, particle->y, PARTICLE_SIZE, PARTICLE_SIZE);
	}
}

