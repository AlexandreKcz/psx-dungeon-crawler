#include "../types.h"

#include <stdio.h>
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

vector2 vector_substract(vector2 *vec1, vector2 *vec2){
	vector2 substraction = {
		.vx = vec1->vx - vec2->vx,
		.vy = vec1->vy - vec2->vy
	};

	return substraction;
}

vector2 vector_multiply(vector2* vec1, vector2* vec2){
	vector2 multiplied = {
		.vx = vec1->vx * vec2->vx,
		.vy = vec1->vy * vec2->vy
	};

	return  multiplied;
}

vector2 vector_divide(vector2* vec1, vector2* vec2){
	vector2 divided = {
		.vx = vec1->vx / vec2->vx,
		.vy = vec1->vy / vec2->vy
	};

	return divided;
}

vector2 vector_uniform_scale(vector2 *vec, int scale){
	vector2 scaled = {
		.vx = vec->vx * scale,
		.vy = vec->vy * scale
	};

	return scaled;
}

vector2_int vector_convert_to_int(vector2* vec){
	vector2_int convert = {
		.vx = (int) vec->vx,
		.vy = (int) vec->vy
	};

	return convert;
}

vector2 vector_convert_from_int(vector2_int* vec){
	//TODO : add a warning if size exceeded
	
	vector2 convert = {
		.vx = (int) vec->vx,
		.vy = (int) vec->vy
	};

	return convert;
}

vector2_int vector_int_add(vector2_int* vec1, vector2_int* vec2){
	vector2_int addition = {
		.vx = vec1->vx + vec2->vx,
		.vy = vec1->vy + vec2->vy
	};

	return  addition;
}

vector2_int vector_int_substract(vector2_int* vec1, vector2_int* vec2){
	vector2_int substraction = {
		.vx = vec1->vx - vec2->vx,
		.vy = vec1->vy - vec2->vy
	};

	return substraction;
}

vector2_int vector_int_multiply(vector2_int* vec1, vector2_int* vec2){
	vector2_int multiplied = {
		.vx = vec1->vx * vec2->vx,
		.vy = vec1->vy * vec2->vy
	};

	return  multiplied;
}

vector2_int vector_int_divide(vector2_int* vec1, vector2_int* vec2){
	vector2_int divided = {
		.vx = vec1->vx / vec2->vx,
		.vy = vec1->vy / vec2->vy
	};

	return divided;
}

vector2_int vector_int_uniform_scale(vector2_int* vec, int scale){
	vector2_int scaled = {
		.vx = vec->vx * scale,
		.vy = vec->vy * scale
	};

	return scaled;
}