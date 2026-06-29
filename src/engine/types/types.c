#include "../types.h"

#include <stdlib.h>

void color_create(int r, int g, int b, Color** color) {
	*color = malloc3(sizeof(Color));
	(*color) -> r = r;
	(*color) -> g = g;
	(*color) -> b = b;
}

void color_free(Color *color){
	if(color == NULL)
		return;

	free3(color);
}