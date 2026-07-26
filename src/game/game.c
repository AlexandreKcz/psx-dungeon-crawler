#include "game.h"

#include <stdio.h>
#include <libgpu.h>
#include <stddef.h>

#include "../engine/types.h"
#include "../engine/prims/prims.h"
#include "../engine/controller.h"
#include "../engine/render/render_internal.h"

int cnt = 0;
int size = 25;
Color* white;
Box* box[25];

vector2 offset = { .vx = 50, .vy = 50 };

short dungeon[5][5][4] = {
    { {0,0,1,0}, {0,0,1,0}, {1,1,1,1}, {0,0,1,0},{0,0,1,0} },
    { {0,0,0,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},{0,1,0,0} },
    { {0,0,0,1}, {0,0,0,0}, {0,1,0,0}, {0,0,0,0},{0,1,0,0} },
    { {0,0,0,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},{0,1,0,0} },
    { {1,0,0,0}, {1,1,1,1}, {1,0,0,0}, {1,0,0,0},{1,0,0,0} },
};

void game_start(){
    cnt = 0;
    white = NULL;
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

void game_update(){
    FntPrint("Babaji : %d", cnt++);

}