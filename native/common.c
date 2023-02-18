#include <stdint.h>
#include <math.h>

#include "common.h"

float brightness = 1.0;

struct color rgb(uint8_t red, uint8_t green, uint8_t blue)
{
	return (struct color) {
		.red = red,
		.green = green,
		.blue = blue,
	};
}

void apply_brightness(struct color *color, float brightness)
{
	color->red *= brightness;
	color->green *= brightness;
	color->blue *= brightness;
}

