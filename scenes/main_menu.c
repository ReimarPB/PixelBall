#include "../native/common.h"
#include "../ui/button.h"
#include "transition.h"

static sprite_t sprite_logo, sprite_background;

void play()
{
	transition_to_scene(SCENE_GAME);
}

static struct button play_button = {
	.text = "PLAY",
	.position = POS(WIDTH_PX / 2, 250, H_ALIGN_CENTER, V_ALIGN_TOP),
	.callback = play,
	.type = BUTTON_TYPE_STANDARD,
};

void init_main_menu()
{
	// TODO unlaod
	sprite_logo = load_sprite(SPRITE_LOGO);
	sprite_background = load_sprite(SPRITE_BACKGROUND);

	add_button(play_button);
}

void draw_main_menu(int x, int y, int width, int height)
{
	draw_partial_sprite(sprite_background, x, y, x, y, width, height);

	draw_sprite(sprite_logo, WIDTH_PX / 2 - get_sprite_width(sprite_logo) / 2, 50);
}

