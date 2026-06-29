#include "../types.h"

#include <stdlib.h>

void color_create(int r, int g, int b, Color** color) {
	*color = malloc(sizeof(Color));
	(*color) -> r = r;
	(*color) -> g = g;
	(*color) -> b = b;
}