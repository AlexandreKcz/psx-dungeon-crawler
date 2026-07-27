#include "game.h"

#include <stdio.h>
#include <libgpu.h>
#include <stddef.h>

#include "../engine/types.h"
#include "../engine/prims/prims.h"
#include "../engine/render/render_internal.h"

int cnt = 0;

#include "dungeon-crawler/dungeon_crawler.h"

void game_start(){
    cnt = 0;

    create_player();
    draw_dungeon();
}

void game_update(){
    //FntPrint("Babaji : %d", cnt++);
    FntPrint("Direction : %d Position : %d, %d", 
                    get_player()->direction, 
                    get_player()->position.vx, 
                    get_player()->position.vy
    );
    
    draw_player();
    player_input();
}