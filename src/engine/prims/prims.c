#include "prims.h"

#include <stdlib.h>

#include "../render.h"

void line_create(vector2 start_pos, vector2 end_pos, Color color, Line** line) {
    *line = malloc3(sizeof(Line));
    line_init(start_pos, end_pos, color, *line);
    line_register(*line);
}

void line_init(vector2 start_pos, vector2 end_pos, Color color, Line* line){
    SetLineF2(line);
    setRGB0(line, color.r, color.g, color.b);
    setXY2(line, start_pos.vx, start_pos.vy, end_pos.vx, end_pos.vy);
}

void line_move(vector2 start_pos, vector2 end_pos, Line* line){
    line->x0 = start_pos.vx;
    line->y0 = start_pos.vy;
    line->x1 = end_pos.vx;
    line->y1 = end_pos.vy;
}

void line_draw(Line* line) {
    DrawPrim(line);
}

void line_free(Line *line){
    if(line == NULL)
        return;
    
    free3(line);
}

void box_create(vector2 pos, vector2 dim, Color color, Box** box){
    *box = malloc3(sizeof(Box));
    box_init(pos, dim, color, *box);
}

void box_init(vector2 pos, vector2 dim, Color color, Box* box) {
    vector2 topLeft = { .vx = pos.vx, .vy = pos.vy };
    vector2 topRight = { .vx = (pos.vx + dim.vx), .vy = pos.vy };
    vector2 botRight = { .vx = (pos.vx + dim.vx), .vy = (pos.vy + dim.vy) };
    vector2 botLeft = { .vx = pos.vx, .vy = (pos.vy + dim.vy) };

    line_create(topLeft,    topRight,   color,  &box->lines[0]);
    line_create(topRight,   botRight,   color,  &box->lines[1]);
    line_create(botRight,   botLeft,    color,  &box->lines[2]);
    line_create(botLeft,    topLeft,    color,  &box->lines[3]);

    box->dimensions = dim;
    box->color = color;
}

void box_move(vector2 newPos, Box* box){
    box_init(newPos, box->dimensions, box->color, box);
}

void box_draw(Box* box){
    for(int i = 0; i < 4; i++)
        line_draw(*(box->lines + i));
}

void box_free(Box* box){
    if(box == NULL)
        return;

    for(int i = 0; i < 4; i++)
        line_free(*(box->lines+i));

    free3(box);
}