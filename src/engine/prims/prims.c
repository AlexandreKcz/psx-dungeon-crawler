#include "../prims.h"

#include <stdio.h>
#include <stdlib.h>

#include "../render.h"

/**
 * @brief create a Line prims, allocate and initialize it and register it in the lines array of the renderer
 * 
 * @param start_pos screen coordinate of first line point
 * @param end_pos   screen coordinates of last line point
 * @param color     color value of the line
 * @param line      double pointer to the line to initialize
 */
void line_create(vector2 start_pos, vector2 end_pos, Color color, Line** line) {
    *line = malloc3(sizeof(Line));
    line_init(start_pos, end_pos, color, *line);
    line_register(*line);
}

/**
 * @brief initialize a line prim, but it is not registered in the lines array of the renderer
 * 
 * @param start_pos screen coordinate of first line point
 * @param end_pos   screen coordinate of the last line point
 * @param color     color valuer of the line
 * @param line      double pointer to the line to initialize
 */
void line_init(vector2 start_pos, vector2 end_pos, Color color, Line* line){
    SetLineF2(line);
    setRGB0(line, color.r, color.g, color.b);
    setXY2(line, start_pos.vx, start_pos.vy, end_pos.vx, end_pos.vy);
}

/**
 * @brief change the screen position of the two line points
 * 
 * @param start_pos new coordinate of first line point
 * @param end_pos   new coordinate of last line point
 * @param line      pointer to the line to modify
 */
void line_move(vector2 start_pos, vector2 end_pos, Line* line){

    line->x0 = start_pos.vx;
    line->y0 = start_pos.vy;
    line->x1 = end_pos.vx;
    line->y1 = end_pos.vy;
}

/**
 * @brief wrapper for Psy-Q's DrawPrim funcion
 * 
 * @param line line to draw
 */
void line_draw(Line* line) {
    DrawPrim(line);
}

/**
 * @brief destroy and free allocated ram for line
 * 
 * @param line pointer to the line to destroy
 */
void line_free(Line *line){
    if(line == NULL)
        return;

    //TODO : Unregister the line in renderer's lines array

    free3(line);
}

/**
 * @brief create a Box prim, allocate and initialize it
 * 
 * @param pos   screen coordinate of the top left point
 * @param dim   dimension of the box
 * @param color color value of the box
 * @param box   double pointer to the box to initialize
 */
void box_create(vector2 pos, vector2 dim, Color color, Box** box){
    *box = malloc3(sizeof(Box));
    box_init(pos, dim, color, *box);
}

/**
 * @brief initialize a Box prim
 * 
 * @param pos   screen coordinate of the top left point
 * @param dim   dimension of the box
 * @param color color value of the box
 * @param box   pointer to the box to initialize
 */
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

/**
 * @brief #DEPRECATED : create a box with a mask to hide certain edge
 * 
 * @param pos   screen coordinate of the top left point
 * @param dim   dimension of the box
 * @param color color value of the box
 * @param mask  must be a list of 4 shorts that represent edges in this order :  [ Top, Right, Bottom, Left ] Values greater than 0 are hidden edges
 * @param box   double pointer to the box to initialize
 */
void box_create_mask(vector2 pos, vector2 dim, Color color, short mask[], Box** box){
    *box = malloc3(sizeof(Box));
    box_init_mask(pos, dim, color, mask, *box);
}

/**
 * @brief #DEPRECATED : same as box init, initialize a box prim but using a mask
 * 
 * @param pos   screen coordinate of the top left point
 * @param dim   dimension of the box
 * @param color color value of the box
 * @param mask  must be a list of 4 shorts that represent edges in this order :  [ Top, Right, Bottom, Left ] Values greater than 0 are hidden edges
 * @param box   pointer to the box to initialize
 */
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

/**
 * @brief change the screen coordinate of the box
 * 
 * @param newPos    screen coordinate of the top left point
 * @param box       pointer to the box to move
 */
void box_set_position(vector2 newPos, Box* box){

    vector2 topLeft = { .vx = newPos.vx, .vy = newPos.vy };
    vector2 topRight = { .vx = (newPos.vx + box->dimensions.vx), .vy = newPos.vy };
    vector2 botRight = { .vx = (newPos.vx + box->dimensions.vx), .vy = (newPos.vy + box->dimensions.vy) };
    vector2 botLeft = { .vx = newPos.vx, .vy = (newPos.vy + box->dimensions.vy) };

    line_move(topLeft,    topRight,   box->lines[0]);
    line_move(topRight,   botRight,   box->lines[1]);
    line_move(botRight,   botLeft,    box->lines[2]);
    line_move(botLeft,    topLeft,    box->lines[3]);
}

/**
 * @brief draw the four lines of the box
 * 
 * @param box pointer to the box to draw
 */
void box_draw(Box* box){
    for(int i = 0; i < 4; i++)
        line_draw(*(box->lines + i));
}

/**
 * @brief destroy and free allocated ram for box
 * 
 * @param box pointer to the box to destroy
 */
void box_free(Box* box){
    if(box == NULL)
        return;

    for(int i = 0; i < 4; i++)
        line_free(*(box->lines+i));

    free3(box);
}