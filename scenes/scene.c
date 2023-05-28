#include "scene.h"
#include "main_menu.h"
#include "../ui/button.h"
#include "../native/common.h"

void change_scene(enum scene s)
{
	clear_buttons();
	scene = s;

	switch (scene) {
		case SCENE_MAIN_MENU: init_main_menu(); break;
		default: break;
	}

	redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
}

