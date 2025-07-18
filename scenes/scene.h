#ifndef SCENE_H
#define SCENE_H

enum scene {
	SCENE_GAME,
	SCENE_PAUSE,
	SCENE_MAIN_MENU,
	SCENE_TRANSITION,
	SCENE_LEVEL_SELECT,
};

extern enum scene scene;

void change_scene(enum scene scene);

void load_scene(enum scene scene);

#endif

