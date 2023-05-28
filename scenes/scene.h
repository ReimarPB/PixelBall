enum scene {
	SCENE_GAME,
	SCENE_PAUSE,
	SCENE_MAIN_MENU,
};

extern enum scene scene;

void change_scene(enum scene scene);

