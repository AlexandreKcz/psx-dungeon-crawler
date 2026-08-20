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

short player_lookat_matrix[3][7][4];

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
            vector2 pos = { .vx = offset.vx + x * size, .vy = offset.vy + y * size };
            vector2 dim = { .vx = size, .vy = size };

            box_create_mask(pos, dim, *white, dungeon[y][x], &box[y + x]);
            /*
            for(int w = 0; w < 4; w++){

                vector2 pos = { .vx = offset.vx + x * size, .vy = offset.vy + y * size };
                vector2 dim = { .vx = size, .vy = size };

                box_create_mask(pos, dim, *white, dungeon[y][x], &box[y + x]);
            }
            */
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

vector2 get_direction_vector(int dir){

    vector2 dir_vec = { .vx = 0, .vy = 0 };
    
    switch (dir){
        case 0: //north
            dir_vec.vy = -1;
            dir_vec.vx = 0;
            break;
        case 1: //east
            dir_vec.vy = 0;
            dir_vec.vx = 1;
            break;
        case 2: //south
            dir_vec.vy = 1;
            dir_vec.vx = 0;
            break;
        case 3: //west
            dir_vec.vy = 0;
            dir_vec.vx = -1;
            break;
    }

    return dir_vec;
}

vector2 player_direction_vector(){
    return  get_direction_vector(player->direction);
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

void set_player_lookat_matrix(short matrix[3][7][4]){

    /* Elegant mais inutilement complique en fin de compte
    vector2 matrix_half_dim = { .vx = 3, .vy = 2 };
    vector2 player_forward_vector = player_direction_vector();

    vector2 player_left_vector = get_direction_vector((player->direction - 1) % 4);
    vector2 scaled_left_vector = vector_multiply(&player_left_vector, &matrix_half_dim);
    vector2 start_coord = vector_add(&player->position, &scaled_left_vector);

    vector2 scaled_right_vector = vector_uniform_scale(&scaled_left_vector, -1);
    vector2 scaled_forward_vector = vector_multiply(&player_forward_vector, &matrix_half_dim);
    vector2 cumulated_vector = vector_add(&scaled_right_vector, &scaled_forward_vector);
    vector2 end_coord = vector_add(&player->position, &cumulated_vector);
    */

    static short surrounding_matrix[7][7][4];
    vector2 dungeon_dim = { .vx = (sizeof(dungeon[0]) / sizeof(dungeon[0][0])), .vy = (sizeof(dungeon) / sizeof(dungeon[0])) };
    int mat_y = 0, mat_x = 0;

    for(int y = player->position.vy - 3; y <= player->position.vy + 3; y++){
        mat_x = 0;
        for(int x = player->position.vx - 3; x <= player->position.vx + 3; x++){
            for(int w = 0; w < 4; w++){
                if(x < 0 || y < 0 || x >= dungeon_dim.vx || y >= dungeon_dim.vy)
                    surrounding_matrix[mat_y][mat_x][w] = 0;
                else
                    surrounding_matrix[mat_y][mat_x][w] = dungeon[y][x][w];
            }

            mat_x++;
        }
        mat_y++;
    }

    for (int d = 0; d < player->direction; d++){
        rotate_matrix(7, surrounding_matrix);
    }

    for(int y = 0; y < 7; y++){
        for(int x = 0; x < 7; x++){
            rotate_array(surrounding_matrix[y][x], 4, player->direction);
        }
    }

    for(int look_y = 0; look_y < 3; look_y++){
        int src_y = 3 - look_y;
        for(int x = 0; x < 7; x++){
            for(int w = 0; w < 4; w++){
                matrix[look_y][x][w] = surrounding_matrix[src_y][x][w];
            }
        }
    }

    for(int y = 0; y < 7; y++){
        for(int x = 0; x < 7; x++){
            for(int w = 0; w < 4; w++)
            {
                if(surrounding_matrix[y][x][w] > 0) printf(".");
                else printf(" ");
            }
                //printf("%u",matrix[y][x][w]);
            printf("|");
        }
        printf("\n");
    }
    
    printf("\n");


    /*
    for(int y = 1; y < 4; y++){
        for(int x = 0; x < 7; x++){
            for(int w = 0; w < 4; w++)
                matrix[y-1][x][w] = surrounding_matrix[y][x][w];
        }
    }
    */
};

void display_matrix(short matrix[3][7][4]){

    /*
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 7; x++){
            for(int w = 0; w < 4; w++)
            {
                if(matrix[y][x][w] > 0) printf(".");
                else printf(" ");
            }
                //printf("%u",matrix[y][x][w]);
            printf("|");
        }
        printf("\n");
    }

    printf("\n");
    */

    for(int wall_index = 0; wall_index < 20; wall_index++)
    {
        if(
            player_lookat_matrix[wall_sprite_matrix[wall_index][1]][wall_sprite_matrix[wall_index][0]][wall_sprite_matrix[wall_index][2]] > 0 ||
            player_lookat_matrix[wall_sprite_matrix[wall_index][4]][wall_sprite_matrix[wall_index][3]][wall_sprite_matrix[wall_index][5]] > 0
        )
            sprite_set_active(walls[wall_index], 1);
        else
            sprite_set_active(walls[wall_index], 0);
    }
}

vector2 scale = VECTOR_ONE;

void player_input(){
    int input_pressed = 0;

    if(pad_check_pressed(pad1Right)){
        if(player->direction == 3)
            player->direction = 0;
        else
            player->direction++;

        input_pressed = 1;
    }

    if(pad_check_pressed(pad1Left)){
        if(player->direction == 0)
            player->direction = 3;
        else
            player->direction--;

        input_pressed = 1;
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

        input_pressed = 1;
    }

    if(input_pressed > 0){
        set_player_lookat_matrix(player_lookat_matrix);
        display_matrix(player_lookat_matrix);
    }

    //vector2 movement = {.vx = 1, .vy = 0};
    scale.vx++;
    scale.vy++;
    sprite_set_scale_vector(bg, scale);
    //sprite_move_vector(bg, movement);
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

    for(int w = 0; w < 20; w++){
        //printf("\nSetting active\n");
        sprite_set_active(walls[w], 0);
        //printf("\nSetting parent\n");
        sprite_link(bg, walls[w]);
    };

    /*
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

    walls[14]->active = 0;
    walls[15]->active = 0;
    walls[16]->active = 0;
    walls[17]->active = 0;
    walls[18]->active = 0;
    walls[19]->active = 0;
    */

    //sprite_link(bg, walls[0]);

    /*
    for(int w = 0; w < 20; w++){
        sprite_link(bg, walls[w]);
    };
    */

    //sprite_flip_vertical(bg, 1);
    //sprite_flip_horizontal(walls[0], 1);
    printf(" Background : %d\n", bg->active);
}

//https://www.geeksforgeeks.org/dsa/inplace-rotate-square-matrix-by-90-degrees/
void rotate_matrix(short n, short matrix[7][7][4]){
    static int result[7][7][4];

    /*
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                result[n - j - 1][i][k] = matrix[i][j][k];
        }
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                matrix[i][j][k] = result[i][j][k];
        }
    }
    */

    /*
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                result[j][n - 1 - i][k] = matrix[i][j][k];
        }
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                matrix[i][j][k] = result[i][j][k];
        }
    }
    */

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                result[n - 1 - j][i][k] = matrix[i][j][k];
        }
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for(int k = 0; k < 4; k++)
                matrix[i][j][k] = result[i][j][k];
        }
    }
}

//https://www.geeksforgeeks.org/dsa/c-program-cyclically-rotate-array-one/
//https://www.geeksforgeeks.org/dsa/array-rotation/
void rotate_array(short array[], int n, int d){
    for(int i = 0; i < d; i++){
        int first = array[0];
        for(int j = 0; j < n - 1; j++){
            array[j] = array[j + 1];
        }
        array[n - 1] = first;
    }
}