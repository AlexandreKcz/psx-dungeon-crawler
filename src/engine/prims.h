#pragma once

#include <libgpu.h>

#include "types.h"

/**
 * @brief structure that represent a box prims with 4 lines to draw
 */
typedef struct {
    Line* lines[4];
    vector2 dimensions;
    Color color;
} Box;

void line_create(vector2 start_pos, vector2 end_pos, Color color, Line** line);
void line_init(vector2 start_pos, vector2 end_pos, Color color, Line* line);
void line_move(vector2 start_pos, vector2 end_pos, Line* line);
void line_draw(Line* line);
void line_free(Line* line);

void box_create(vector2 pos, vector2 dim, Color color, Box** box);
void box_init(vector2 pos, vector2 dim, Color color, Box* box);
void box_create_mask(vector2 pos, vector2 dim, Color color, short mask[], Box** box);
void box_init_mask(vector2 pos, vector2 dim, Color color, short mask[], Box* box);
void box_set_position(vector2 newPos, Box* box);
void box_draw(Box* box);
void box_free(Box* box);