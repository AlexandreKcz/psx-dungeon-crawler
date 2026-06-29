#include "game.h"

#include <stdio.h>
#include <libgpu.h>

int cnt;

void game_start(){
    cnt = 0;
    printf("This has been called from game start \n");
}

void game_update(){
    FntPrint("Babaji : %d", cnt++);
}