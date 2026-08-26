#pragma once

#include <libgpu.h>

typedef struct {
    int r;
    int g;
    int b;
} Color ;

typedef LINE_F2 Line;
typedef DVECTOR vector2;
typedef struct {
    int vx;
    int vy;
} vector2_int;

static const vector2 VECTOR_ONE = { .vx = ONE, .vy = ONE };

void color_create(int r, int g, int b, Color** color);
void color_free(Color* color);

vector2 vector_add(vector2* vec1, vector2* vec2);
vector2 vector_substract(vector2* vec1, vector2* vec2);
vector2 vector_multiply(vector2* vec1, vector2* vec2);
vector2 vector_divide(vector2* vec1, vector2* vec2);
vector2 vector_uniform_scale(vector2* vec, int scale);

vector2_int vector_convert_to_int(vector2* vec);
vector2 vector_convert_from_int(vector2_int* vec);
vector2_int vector_int_add(vector2_int* vec1, vector2_int* vec2);
vector2_int vector_int_substract(vector2_int* vec1, vector2_int* vec2);
vector2_int vector_int_multiply(vector2_int* vec1, vector2_int* vec2);
vector2_int vector_int_divide(vector2_int* vec1, vector2_int* vec2);
vector2_int vector_int_uniform_scale(vector2_int* vec, int scale);

vector2 vector_cross_multiply(vector2* s1, vector2* s2, vector2* m);