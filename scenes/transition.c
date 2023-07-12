#include "../native/common.h"
#include "../globals.h"
#include "../main.h"
#include "scene.h"

#define TRANSITION_SPEED 15

enum scene old_scene, new_scene;

// Starts at the window height, and is continuously decremented
// When it reaches zero, the scene changes
// When it reaches the negative window height, the transition is done
int progress;

void transition_to_scene(enum scene new)
{
	old_scene = scene;
	new_scene = new;
	change_scene(SCENE_TRANSITION);
	progress = HEIGHT_PX;
}

void update_transition()
{
	progress -= TRANSITION_SPEED;

	if (progress == 0) init_scene(new_scene);
	if (progress == -HEIGHT_PX) scene = new_scene;

	redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
}

void draw_transition(int x, int y, int width, int height)
{
	int scene_start  = progress > 0 ? HEIGHT_PX - progress : 0;
	int scene_height = progress > 0 ? progress : -progress;

	int black_start  = progress > 0 ? 0 : -progress;
	int black_height = progress > 0 ? HEIGHT_PX - progress : HEIGHT_PX + progress;

	scene = progress > 0 ? old_scene : new_scene;
	draw(0, scene_start, WIDTH_PX, scene_height);
	scene = SCENE_TRANSITION;

	draw_rect(rgb(0, 0, 0), 0, black_start, WIDTH_PX, black_height);
}

