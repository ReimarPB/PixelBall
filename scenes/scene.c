#include "scene.h"
#include "main_menu.h"
#include "level_select.h"
#include "../ui/button.h"
#include "../native/common.h"

void change_scene(enum scene s)
{
	clear_buttons();
	scene = s;
	load_scene(s);
	redraw_area(0, 0, WIDTH_PX, HEIGHT_PX);
}

void load_scene(enum scene s)
{
       switch (s) {
		case SCENE_MAIN_MENU:    load_main_menu(); break;
		case SCENE_LEVEL_SELECT: load_level_select(); break;
		default: break;
       }
}

