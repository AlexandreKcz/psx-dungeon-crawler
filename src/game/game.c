#include "game.h"

#include <stdio.h>
#include <libgpu.h>
#include <stddef.h>

#include "../engine/types.h"
#include "../engine/prims/prims.h"
#include "../engine/controller.h"
#include "../engine/render/render_internal.h"

int cnt;
Color* white;
Box* box;

vector2 pos = { .vx = SCREEN_WIDTH_COMMON / 2, .vy = SCREEN_HEIGHT_PAL / 2 };

Line* lines[25];

void game_start(){
    cnt = 0;
    printf("This has been called from game start \n");

    white = NULL;
    box = NULL;

    vector2 dim = { .vx = 50, .vy = 50 };

    color_create(255, 255, 255, &white);
    printf("Creating Box : \n");
    box_create(pos, dim, *white, &box);
    
    printf("Creating Lines : \n");
    /*
    for(int i = 0; i < 25; i++)
        line_create(pos, dim, *white, &lines[i]);
    */
}

void game_update(){
    FntPrint("Babaji : %d", cnt++);

    if(pad_check(pad1Right)){
        pos.vx += 1;
    }
    if(pad_check(pad1Left)){
        pos.vx -= 1;
    }
    if(pad_check(pad1Up)){
        pos.vy -= 1;
    }
    if(pad_check(pad1Down)){
        pos.vy += 1;
    }

    box_move(pos, box);
}