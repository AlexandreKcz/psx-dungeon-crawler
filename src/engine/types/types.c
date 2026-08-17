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

vector2 vector_add(vector2* vec1, vector2* vec2){
	vector2 addition = {
		.vx = vec1->vx + vec2->vx,
		.vy = vec1->vy + vec2->vy
	};

	return  addition;
}

vector2 vector_multiply(vector2* vec1, vector2* vec2){
	vector2 multiplied = {
		.vx = vec1->vx * vec2->vx,
		.vy = vec1->vy * vec2->vy
	};

	return  multiplied;
}

vector2 vector_uniform_scale(vector2 *vec, int scale){
	vector2 scaled = {
		.vx = vec->vx * scale,
		.vy = vec->vy * scale
	};

	return scaled;
}