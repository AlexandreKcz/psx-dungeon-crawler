#include "prims.h"

#include <stdio.h>
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

void box_create_mask(vector2 pos, vector2 dim, Color color, short mask[], Box** box){
    *box = malloc3(sizeof(Box));
    box_init_mask(pos, dim, color, mask, *box);
}

void box_init_mask(vector2 pos, vector2 dim, Color color, short mask[], Box* box){

    if(sizeof(&mask) / sizeof(short) > 4){
        printf("Invalid mask lenght to create box, mask must be a 4 short array");
        return;
    }

    vector2 topLeft = { .vx = pos.vx, .vy = pos.vy };
    vector2 topRight = { .vx = (pos.vx + dim.vx), .vy = pos.vy };
    vector2 botRight = { .vx = (pos.vx + dim.vx), .vy = (pos.vy + dim.vy) };
    vector2 botLeft = { .vx = pos.vx, .vy = (pos.vy + dim.vy) };

    if(mask[0] > 0)
        line_create(topLeft,    topRight,   color,  &box->lines[0]);
    if(mask[1] > 0)
        line_create(topRight,   botRight,   color,  &box->lines[1]);
    if(mask[2] > 0)
        line_create(botRight,   botLeft,    color,  &box->lines[2]);
    if(mask[3] > 0)
        line_create(botLeft,    topLeft,    color,  &box->lines[3]);

    box->dimensions = dim;
    box->color = color;
}

void box_move(vector2 newPos, Box* box){
    //box_init(newPos, box->dimensions, box->color, box);

    vector2 topLeft = { .vx = newPos.vx, .vy = newPos.vy };
    vector2 topRight = { .vx = (newPos.vx + box->dimensions.vx), .vy = newPos.vy };
    vector2 botRight = { .vx = (newPos.vx + box->dimensions.vx), .vy = (newPos.vy + box->dimensions.vy) };
    vector2 botLeft = { .vx = newPos.vx, .vy = (newPos.vy + box->dimensions.vy) };

    line_move(topLeft,    topRight,   box->lines[0]);
    line_move(topRight,   botRight,   box->lines[1]);
    line_move(botRight,   botLeft,    box->lines[2]);
    line_move(botLeft,    topLeft,    box->lines[3]);
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