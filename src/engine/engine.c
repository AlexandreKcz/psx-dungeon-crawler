#include "engine.h"

#include <stdio.h>

#include "system/system.h"
#include "render.h"
#include "constants.h"
#include "../game/game.h"

void initialize(){
    //system.c
    initialize_memory_heap();

    //render.c
    initialize_screen();
    initialize_debug_font();

    //initialize pad
    //load cd data

    printf("End of game initialization \n");
}

void start(){

    game_start();

    printf("Started version of game : %d.%d succesfully \n", GAME_MAJOR_VERSION, GAME_MINOR_VERSION);
}

void update()
{
    //run pad_update();

    game_update();

    //render.c
    clear_display();
    draw();
    display();
}