#pragma once

typedef struct {
    int r;
    int g;
    int b;
} Color ;

void color_create(int r, int g, int b, Color** color);