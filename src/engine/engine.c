#include "engine.h"

#include <stdio.h>

#include "system.h"
#include "render.h"
#include "constants.h"
#include "controller.h"
#include "../game/game.h"

/**
 * @brief handle engine initialization : init memory heap, init screen and font, init pad
 * 
 */
void _engine_initialize(){
    //system.c
    _initialize_memory_heap();

    //render.c
    _initialize_screen();
    initialize_debug_font();

    //controller.c
    _pad_initialize();

    printf("End of game initialization \n");
}

/**
 * @brief wrapper to call game_start from engine and display game version
 * 
 */
void _engine_start(){

    game_start();

    printf("Started version of game : %d.%d succesfully \n", GAME_MAJOR_VERSION, GAME_MINOR_VERSION);
}

/**
 * @brief handle engine update : update pad, call game_update and update render
 * 
 */
void _engine_update()
{
    _pad_update();

    game_update();

    _render_update();
}