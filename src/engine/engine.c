#include "engine.h"

#include <stdio.h>

#include "system/system.h"
#include "render.h"
#include "constants.h"
#include "controller.h"
#include "../game/game.h"

void initialize(){
    //system.c
    initialize_memory_heap();

    //render.c
    initialize_screen();
    initialize_debug_font();

    //controller.c
    pad_initialize();
    //load cd data

    printf("End of game initialization \n");
}

void start(){

    game_start();

    printf("Started version of game : %d.%d succesfully \n", GAME_MAJOR_VERSION, GAME_MINOR_VERSION);
}

void update()
{
    pad_update();

    game_update();

    //render.c
    render_update();
    /*
    clear_display();
    draw();
    display();
    */
}