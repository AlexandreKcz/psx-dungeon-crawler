#pragma once

#include <libgpu.h>

typedef struct {
    int r;
    int g;
    int b;
} Color ;

typedef LINE_F2 Line;
typedef DVECTOR vector2;

void color_create(int r, int g, int b, Color** color);
void color_free(Color* color);

vector2 vector_add(vector2* vec1, vector2* vec2);