#include <stdlib.h>

#include "../native/common.h"
#include "../ui/button.h"
#include "../globals.h"
#include "../main.h"
#include "scene.h"

#define TRANSITION_SPEED 15
#define TRANSITION_CHUNK_WIDTH 70
#define TRANSITION_CHUNK_MAX_HEIGHT 60
#define TRANSITION_CHUNK_AMOUNT (WIDTH_PX / TRANSITION_CHUNK_WIDTH)
#define TRANSITION_FULL_HEIGHT (HEIGHT_PX + TRANSITION_CHUNK_MAX_HEIGHT)

enum scene old_scene, new_scene;

// Starts at the window height, and is continuously decremented
// When it reaches zero, the scene changes
// When it reaches the negative window height, the transition is done
int progress;

int chunk_offsets[TRANSITION_CHUNK_AMOUNT];

void generate_offsets(void)
{
	for (int i = 0; i < TRANSITION_CHUNK_AMOUNT; i++) {
		do {
			chunk_offsets[i] = rand() % TRANSITION_CHUNK_MAX_HEIGHT;
		} while (i > 0 && abs(chunk_offsets[i] - chunk_offsets[i-1]) < 10); // Ensure enough height difference between chunks
	}
}

void transition_to_scene(enum scene new)
{
	old_scene = scene;
	new_scene = new;

	scene = SCENE_TRANSITION;

	progress = TRANSITION_FULL_HEIGHT;

	generate_offsets();
}

void update_transition(void)
{
	progress -= TRANSITION_SPEED;

	if (progress == 0) {
		clear_buttons();
		load_scene(new_scene);
		generate_offsets();
	}

	if (progress == -TRANSITION_FULL_HEIGHT) {
		scene = new_scene;
		redraw_area(
			0,
			HEIGHT_PX - TRANSITION_CHUNK_MAX_HEIGHT - TRANSITION_SPEED,
			WIDTH_PX,
			TRANSITION_CHUNK_MAX_HEIGHT + TRANSITION_SPEED
		);
		return;
	}

	// Update scene
	scene = progress > 0 ? old_scene : new_scene;
	update();
	scene = SCENE_TRANSITION;

	redraw();
}

void draw_transition(void)
{
	int black_start  = progress > 0 ? 0 : -progress;
	int black_height = progress > 0 ? HEIGHT_PX - progress : TRANSITION_FULL_HEIGHT + progress;

	// Draw scene area
	scene = progress > 0 ? old_scene : new_scene;
	draw();
	scene = SCENE_TRANSITION;

	// Draw black transition area
	for (int i = 0; i < TRANSITION_CHUNK_AMOUNT; i++) {
		int chunk_start  = progress > 0 ? black_start : black_start - chunk_offsets[i];
		int chunk_height = progress > 0 ? black_height + chunk_offsets[i] : black_height;

		if (chunk_height < 0) continue;

		draw_rect(rgb(0, 0, 0), i * TRANSITION_CHUNK_WIDTH, chunk_start, TRANSITION_CHUNK_WIDTH, chunk_height);
	}
}

