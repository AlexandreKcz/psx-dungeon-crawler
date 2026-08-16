#include "dungeon_crawler.h"

#include "../../engine/prims/prims.h"
#include "../../engine/controller.h"
#include "../../engine/sprite.h"
#include <stdlib.h>
#include <stdio.h>

int size = 25;

Player* player;
Color* white;
Color* red;
Box* box[25];
vector2 offset = { .vx = 50, .vy = 50 };

Sprite* bg;
Sprite* walls[20];

short dungeon[5][5][4] = {
    { {0,0,1,0}, {0,0,1,0}, {1,1,1,1}, {0,0,0,0},{0,0,1,0} },
    { {0,0,0,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},{0,1,0,0} },
    { {0,0,0,1}, {0,0,0,1}, {0,0,0,0}, {0,0,0,0},{0,1,0,0} },
    { {0,0,0,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},{0,1,0,0} },
    { {1,0,0,0}, {1,1,1,1}, {1,0,0,0}, {1,0,0,0},{1,0,0,0} },
};

unsigned short wall_sprite_matrix[20][6] = {
     {2,0,1,3,0,3},
     {3,0,1,4,0,3},
     {2,0,0,2,1,2},
     {3,0,0,3,1,2},
     {4,0,0,4,1,2},
     {1,1,1,2,1,3},
     {2,1,1,3,1,3},
     {3,1,1,4,1,3},
     {4,1,1,5,1,3},
     {1,1,0,1,2,2},
    {2,1,0,2,2,2},
    {3,1,0,3,2,2},
    {4,1,0,4,2,2},
    {5,1,0,5,2,2},
    {0,2,1,1,2,3},
    {1,2,1,2,2,3},
    {2,2,1,3,2,3},
    {3,2,1,4,2,3},
    {4,2,1,5,2,3},
    {5,2,1,6,2,3},
};

void create_player(){
    player = malloc(sizeof(Player));

    color_create(255, 0, 0, &red);

    vector2 pos = { .vx = 0, .vy = 2 };
    player->position = pos;
    player->direction = 0;

    line_create(player->position, player->position, *red, &player->visual);
}

void draw_dungeon(){
    color_create(255, 255, 255, &white);

    for (int y = 0; y < 5; y++){
        for(int x = 0; x < 5; x++){
            for(int w = 0; w < 4; w++){

                vector2 pos = { .vx = offset.vx + x * size, .vy = offset.vy + y * size };
                vector2 dim = { .vx = size, .vy = size };

                box_create_mask(pos, dim, *white, dungeon[y][x], &box[y + x]);
            }
        }
    }
}

Player* get_player(){
    return player;
}

void draw_player(){

    vector2 halfCell = { .vx = size / 2, .vy = size / 2 };
    vector2 line_start = {
        .vx = offset.vx + player->position.vx * size + halfCell.vx,
        .vy = offset.vy + player->position.vy * size + halfCell.vy,
    };

    int end_x = 0;
    int end_y = 0;

    switch (player->direction){
        case 0: //north
            end_y = halfCell.vy / -2;
            end_x = 0;
            break;
        case 1: //east
            end_y = 0;
            end_x = halfCell.vx / 2;
            break;
        case 2: //south
            printf("SOUTH");
            end_y = halfCell.vy / 2;
            end_x = 0;
            break;
        case 3: //west
            end_y = 0;
            end_x = halfCell.vx / -2;
            break;
    }

    vector2 line_end = {
        .vx = line_start.vx + end_x,
        .vy = line_start.vy + end_y
    };

    line_move(line_start, line_end, player->visual);
}

vector2 player_direction_vector(){

    vector2 dir = { .vx = 0, .vy = 0 };

    switch (player->direction){
        case 0: //north
            dir.vy = -1;
            dir.vx = 0;
            break;
        case 1: //east
            dir.vy = 0;
            dir.vx = 1;
            break;
        case 2: //south
            dir.vy = 1;
            dir.vx = 0;
            break;
        case 3: //west
            dir.vy = 0;
            dir.vx = -1;
            break;
    }

    return dir;
}

//1 if collision else 
short player_check_collision(){
    vector2 dir = player_direction_vector();

    short current_cell = dungeon[player->position.vy][player->position.vx][player->direction];

    if(current_cell > 0) 
        return 1;

    vector2 lookat_cell_position = vector_add(&dir, &player->position);

    if(
        lookat_cell_position.vx < 0 || lookat_cell_position.vx > (sizeof(dungeon[0]) / sizeof(dungeon[0][0])) || 
        lookat_cell_position.vy < 0 || lookat_cell_position.vy > (sizeof(dungeon[0]) / (sizeof(dungeon[0]) / sizeof(dungeon[0][0])))
    )
        return 1;

    short reversed_dir = (player->direction + 2) % 4;

    short lookat_cell = dungeon[lookat_cell_position.vy][lookat_cell_position.vx][reversed_dir];

    if(lookat_cell > 0) 
        return 1;

    return 0;
}

void player_input(){
    if(pad_check_pressed(pad1Right)){
        if(player->direction == 3)
            player->direction = 0;
        else
            player->direction++;
    }

    if(pad_check_pressed(pad1Left)){
        if(player->direction == 0)
            player->direction = 3;
        else
            player->direction--;
    }

    if(pad_check_pressed(pad1Up)){
        if(player_check_collision() > 0)
            return;

        switch(player->direction){
            case 0:
                player->position.vy--;
                break;
            case 1:
                player->position.vx++;
                break;
            case 2:
                player->position.vy++;
                break;
            case 3:
                player->position.vx--;
                break;
        }
    }
}

void load_dungeon_sprites(){
    bg = sprite_register("DNG_BG.TIM");
    walls[0] = sprite_register("WALL_0-4.TIM");
    walls[1] = sprite_register("WALL_0-4.TIM");

    walls[2] = sprite_register("WALL_1-5.TIM");
    walls[3] = sprite_register("WALL_1-5.TIM");
    walls[4] = sprite_register("WALL_1-5.TIM");

    walls[5] = sprite_register("WALL_2-2.TIM");
    walls[6] = sprite_register("WALL_2-4.TIM");
    walls[7] = sprite_register("WALL_2-4.TIM");
    walls[8] = sprite_register("WALL_2-2.TIM");

    walls[9] = sprite_register("WALL_3-5.TIM");
    walls[10] = sprite_register("WALL_3-5.TIM");
    walls[11] = sprite_register("WALL_3-5.TIM");
    walls[12] = sprite_register("WALL_3-5.TIM");
    walls[13] = sprite_register("WALL_3-5.TIM");

    walls[14] = sprite_register("WALL_4-0.TIM");
    walls[15] = sprite_register("WALL_4-2.TIM");
    walls[16] = sprite_register("WALL_4-4.TIM");
    walls[17] = sprite_register("WALL_4-4.TIM");
    walls[18] = sprite_register("WALL_4-2.TIM");
    walls[19] = sprite_register("WALL_4-0.TIM");


    //printf("Adress of BG : %p\n", bg);
    sprite_list_load();
    //((Sprite*) array_list_get(get_sprite_list(),0))->active = 1;
    bg->active = 1;
    bg->z_index = 255;

    walls[0]->z_index = 5;
    walls[1]->z_index = 5;

    walls[2]->z_index = 10;
    walls[3]->z_index = 10;
    walls[4]->z_index = 10;

    walls[5]->z_index = 15;
    walls[6]->z_index = 15;
    walls[7]->z_index = 15;
    walls[8]->z_index = 15;

    walls[9]->z_index = 20;
    walls[10]->z_index = 20;
    walls[11]->z_index = 20;
    walls[12]->z_index = 20;
    walls[13]->z_index = 20;

    walls[14]->z_index = 25;
    walls[15]->z_index = 25;
    walls[16]->z_index = 25;
    walls[16]->z_index = 25;
    walls[17]->z_index = 25;
    walls[18]->z_index = 25;
    walls[19]->z_index = 25;

    sprite_flip_horizontal(walls[1], 1);
    sprite_set_position(walls[1], bg->sprite_data->w - walls[1]->sprite_data->w, 0); //TODO : add getters for sprite width and height

    sprite_set_position(walls[2], -26, 28);
    sprite_set_position(walls[3], 45, 28);
    sprite_set_position(walls[4], 116, 28);

    sprite_set_position(walls[5], 0, 37);
    sprite_set_position(walls[6], 45, 28);

    sprite_flip_horizontal(walls[7], 1);
    sprite_set_position(walls[7], 97, 28);
    sprite_flip_horizontal(walls[8], 1);
    sprite_set_position(walls[8], 128, 37);

    sprite_set_position(walls[9], -5, 46);
    sprite_set_position(walls[10], 28, 46);
    sprite_set_position(walls[11], 62, 46);
    sprite_set_position(walls[12], 97, 46);
    sprite_set_position(walls[13], 132, 46);

    sprite_set_position(walls[14], 0, 47);
    sprite_set_position(walls[15], 28, 46);
    sprite_set_position(walls[16], 63, 47);
    sprite_flip_horizontal(walls[17], 1);
    sprite_set_position(walls[17], 90, 47);
    sprite_flip_horizontal(walls[18], 1);
    sprite_set_position(walls[18], 113, 47);
    sprite_flip_horizontal(walls[19], 1);
    sprite_set_position(walls[19], 136, 47);

    walls[0]->active = 0;
    walls[1]->active = 0;

    walls[2]->active = 0;
    walls[3]->active = 0;
    walls[4]->active = 0;

    walls[5]->active = 0;
    walls[6]->active = 0;
    walls[7]->active = 0;
    walls[8]->active = 0;

    walls[9]->active  = 0;
    walls[10]->active = 0;
    walls[11]->active = 0;
    walls[12]->active = 0;
    walls[13]->active = 0;

    walls[14]->active = 1;
    walls[15]->active = 1;
    walls[16]->active = 1;
    walls[17]->active = 1;
    walls[18]->active = 1;
    walls[19]->active = 1;

    //sprite_flip_vertical(bg, 1);
    //sprite_flip_horizontal(walls[0], 1);
    printf(" Background : %d\n", bg->active);
}