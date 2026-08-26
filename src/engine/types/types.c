#include "../types.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief create a specific color with RGB values
 * 
 * @param r red value from 0 to 255
 * @param g green value from 0 to 255
 * @param b blue value from 0 to 255
 * @param color double pointer to the color to initialize
 */
void color_create(int r, int g, int b, Color** color) {
	*color = malloc3(sizeof(Color));
	(*color) -> r = r;
	(*color) -> g = g;
	(*color) -> b = b;
}

/**
 * @brief free the color in memory
 * 
 * @param color pointer to the color to free
 */
void color_free(Color *color){
	if(color == NULL)
		return;

	free3(color);
}

/**
 * @brief add two 2D vectors, return their sum as a vector
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2 : sum of the two vectors
 */
vector2 vector_add(vector2* vec1, vector2* vec2){
	vector2 addition = {
		.vx = vec1->vx + vec2->vx,
		.vy = vec1->vy + vec2->vy
	};

	return  addition;
}

/**
 * @brief substract two 2D vectors, return their difference
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2 : difference of the two vectors
 */
vector2 vector_substract(vector2 *vec1, vector2 *vec2){
	vector2 substraction = {
		.vx = vec1->vx - vec2->vx,
		.vy = vec1->vy - vec2->vy
	};

	return substraction;
}

/**
 * @brief multiply two 2D vectors, return ther product
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2 : product of the two vectors
 */
vector2 vector_multiply(vector2* vec1, vector2* vec2){
	vector2 multiplied = {
		.vx = vec1->vx * vec2->vx,
		.vy = vec1->vy * vec2->vy
	};

	return  multiplied;
}

/**
 * @brief divide two 2D vectors, return the quotient
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2 : quotient of the two vectors (rounded as int)
 */
vector2 vector_divide(vector2* vec1, vector2* vec2){
	vector2 divided = {
		.vx = vec1->vx / vec2->vx,
		.vy = vec1->vy / vec2->vy
	};

	return divided;
}

/**
 * @brief multiply a 2D vector with a uniform
 * 
 * @param vec vector to scale
 * @param scale scaling factor
 * @return vector2 : scaled vector
 */
vector2 vector_uniform_scale(vector2 *vec, int scale){
	vector2 scaled = {
		.vx = vec->vx * scale,
		.vy = vec->vy * scale
	};

	return scaled;
}

/**
 * @brief convert short vector to int vector (usually to avoid overflowing values)
 * 
 * @param vec short vector to convert
 * @return vector2_int : converted vector
 */
vector2_int vector_convert_to_int(vector2* vec){
	vector2_int convert = {
		.vx = (int) vec->vx,
		.vy = (int) vec->vy
	};

	return convert;
}

/**
 * @brief convert an int vector to a short vector
 * 
 * @param vec int vector to convert
 * @return vector2 : converted vector
 */
vector2 vector_convert_from_int(vector2_int* vec){
	//TODO : add a warning if size exceeded
	
	vector2 convert = {
		.vx = (int) vec->vx,
		.vy = (int) vec->vy
	};

	return convert;
}

/**
 * @brief add two 2D int vectors, return their sum as a vector
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2_int : sum of the two vectors
 */
vector2_int vector_int_add(vector2_int* vec1, vector2_int* vec2){
	vector2_int addition = {
		.vx = vec1->vx + vec2->vx,
		.vy = vec1->vy + vec2->vy
	};

	return  addition;
}

/**
 * @brief substract two 2D int vectors, return their difference
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2_int : difference of the two vectors
 */
vector2_int vector_int_substract(vector2_int* vec1, vector2_int* vec2){
	vector2_int substraction = {
		.vx = vec1->vx - vec2->vx,
		.vy = vec1->vy - vec2->vy
	};

	return substraction;
}

/**
 * @brief multiply two 2D int vectors, return ther product
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2_int : product of the two vectors
 */
vector2_int vector_int_multiply(vector2_int* vec1, vector2_int* vec2){
	vector2_int multiplied = {
		.vx = vec1->vx * vec2->vx,
		.vy = vec1->vy * vec2->vy
	};

	return  multiplied;
}

/**
 * @brief divide two 2D int vectors, return the quotient
 * 
 * @param vec1 first vector
 * @param vec2 second vector
 * @return vector2_int : quotient of the two vectors (rounded)
 */
vector2_int vector_int_divide(vector2_int* vec1, vector2_int* vec2){
	vector2_int divided = {
		.vx = vec1->vx / vec2->vx,
		.vy = vec1->vy / vec2->vy
	};

	return divided;
}

/**
 * @brief multiply a 2D int vector with a uniform
 * 
 * @param vec vector to scale
 * @param scale scaling factor
 * @return vector2_int : scaled vector
 */
vector2_int vector_int_uniform_scale(vector2_int* vec, int scale){
	vector2_int scaled = {
		.vx = vec->vx * scale,
		.vy = vec->vy * scale
	};

	return scaled;
}

//s1 = parent, s2 = child, m = ONE
/**
 * @brief cross multiplication between two vector for linear scaling, short vectors are temporary converted to int vector to prevent value overflow
 * 
 * @param s1 top left vector of the cross multiplication
 * @param s2 top right vector of the cross multiplaction
 * @param m bottom left vector of the cross multiplication
 * @return vector2 : bottom right vector of the cross multiplication
 */
vector2 vector_cross_multiply(vector2* s1, vector2* s2, vector2* m){

	//TODO : rename parameter to mahtematically suited name (like a,b,c)

    vector2_int s1_convert = vector_convert_to_int(s1);
    vector2_int s2_convert = vector_convert_to_int(s2);
    vector2_int m_convert = vector_convert_to_int(m);

    //CANNOT use short vector multiplication
    vector2_int s2_m_product = vector_int_multiply(&s2_convert, &m_convert);
    //printf("\n product : %d, %d\n", s2_m_product.vx, s2_m_product.vy);

    vector2_int s1_division = vector_int_divide(&s2_m_product, &s1_convert);

    vector2 result = vector_convert_from_int(&s1_division);

    return result;
}