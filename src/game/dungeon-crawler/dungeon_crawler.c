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

    sprite_flip_horizontal(walls[1], 1);
    sprite_set_position(walls[1], bg->sprite_data->w - walls[1]->sprite_data->w, 0); //TODO : add getters for sprite width and height

    sprite_set_position(walls[2], -26, 28);
    sprite_set_position(walls[3], 45, 28);
    sprite_set_position(walls[4], 116, 28);

    sprite_set_position(walls[5], 0, 37);


    walls[0]->active = 0;
    walls[1]->active = 0;

    walls[2]->active = 0;
    walls[3]->active = 0;
    walls[4]->active = 0;

    //sprite_flip_vertical(bg, 1);
    //sprite_flip_horizontal(walls[0], 1);
    printf(" Background : %d\n", bg->active);
}