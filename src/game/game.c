#include "game.h"

#include <stdio.h>
#include <libgpu.h>
#include <stddef.h>

#include "../engine/types.h"
#include "../engine/prims/prims.h"
#include "../engine/render/render_internal.h";

int cnt;
Color* white;
Box* box;

void game_start(){
    cnt = 0;
    printf("This has been called from game start \n");

    white = NULL;
    box = NULL;

    vector2 pos = { .vx = SCREEN_WIDTH_COMMON / 2, .vy = SCREEN_HEIGHT_PAL / 2 };
    vector2 dim = { .vx = 50, .vy = 50 };


    color_create(255, 255, 255, &white);
    box_create(pos, dim, *white, &box);
}

void game_update(){
    FntPrint("Babaji : %d", cnt++);
}

void game_draw(){
    box_draw(box);
}