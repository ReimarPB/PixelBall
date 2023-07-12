#include <stdlib.h>

#include "../native/common.h"
#include "../ui/button.h"
#include "../globals.h"
#include "../main.h"
#include "scene.h"

#define TRANSITION_SPEED 15
#define TRANSITION_CHUNK_WIDTH 70
#define TRANSITION_CHUNK_MAX_HEIGHT 50
#define TRANSITION_CHUNK_AMOUNT (WIDTH_PX / TRANSITION_CHUNK_WIDTH)

enum scene old_scene, new_scene;

// Starts at the window height, and is continuously decremented
// When it reaches zero, the scene changes
// When it reaches the negative window height, the transition is done
int progress;

int chunk_offsets[TRANSITION_CHUNK_AMOUNT];

void transition_to_scene(enum scene new)
{
	old_scene = scene;
	new_scene = new;
	scene = SCENE_TRANSITION;

	progress = HEIGHT_PX;

	// Generate offsets
	for (int i = 0; i < TRANSITION_CHUNK_AMOUNT; i++) {
		do {
			chunk_offsets[i] = rand() % TRANSITION_CHUNK_MAX_HEIGHT;
		} while (i > 0 && abs(chunk_offsets[i] - chunk_offsets[i-1]) < 10); // Ensure enough height difference between chunks
	}
}

void update_transition()
{
	progress -= TRANSITION_SPEED;

	if (progress == 0) {
		clear_buttons();
		init_scene(new_scene);
	}

	if (progress == -HEIGHT_PX) {
		scene = new_scene;
		redraw_area(
			0,
			HEIGHT_PX - TRANSITION_CHUNK_MAX_HEIGHT - TRANSITION_SPEED,
			WIDTH_PX,
			TRANSITION_CHUNK_MAX_HEIGHT + TRANSITION_SPEED
		);
		return;
	}

	redraw_area(0, abs(progress + TRANSITION_SPEED), WIDTH_PX, TRANSITION_SPEED);
}

void draw_transition(int x, int y, int width, int height)
{
	int scene_start  = progress > 0 ? HEIGHT_PX - progress : 0;
	int scene_height = progress > 0 ? progress : -progress;

	int black_start  = progress > 0 ? 0 : -progress;
	int black_height = progress > 0 ? HEIGHT_PX - progress : HEIGHT_PX + progress;

	// Draw scene area
	scene = progress > 0 ? old_scene : new_scene;
	draw(0, scene_start, WIDTH_PX, scene_height);
	scene = SCENE_TRANSITION;

	// Draw black transition area
	for (int i = 0; i < TRANSITION_CHUNK_AMOUNT; i++) {
		draw_rect(
			rgb(0, 0, 0),
			i * TRANSITION_CHUNK_WIDTH,
			progress > 0 ? black_start : black_start - chunk_offsets[i],
			TRANSITION_CHUNK_WIDTH,
			progress > 0 ? black_height + chunk_offsets[i] : black_height
		);
	}
}

