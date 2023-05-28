#include "../native/common.h"
#include "../ui/button.h"
#include "scene.h"

sprite_t logo;

void play()
{
	change_scene(SCENE_GAME);
}

static struct button play_button = {
	.text = "PLAY",
	.position = POS(WIDTH_PX / 2, 250, H_ALIGN_CENTER, V_ALIGN_TOP),
	.callback = play,
	.type = BUTTON_TYPE_STANDARD,
};

void init_main_menu()
{
	logo = load_sprite(SPRITE_LOGO);
	add_button(play_button);
}

void draw_main_menu(int x, int y, int width, int height)
{
	// Background
	draw_rect(
		rgb(129, 212, 250),
		x, y, width, height
	);

	// TODO dont hardcode width
	draw_sprite(logo, WIDTH_PX / 2 - 396 / 2, 50);
}

