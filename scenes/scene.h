#ifndef SCENE_H
#define SCENE_H

enum scene {
	SCENE_GAME,
	SCENE_PAUSE,
	SCENE_MAIN_MENU,
	SCENE_TRANSITION,
};

extern enum scene scene;

void change_scene(enum scene scene);

void init_scene(enum scene scene);

#endif

