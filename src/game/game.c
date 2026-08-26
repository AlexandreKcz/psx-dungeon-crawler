#include "game.h"

#include <stdio.h>
#include <libgpu.h>
#include <stddef.h>

#include "../engine/types.h"
#include "../engine/prims.h"
#include "../engine/render/render_internal.h"
#include "../engine/render.h"

int cnt = 0;

#include "dungeon-crawler/dungeon_crawler.h"

void game_start(){
    cnt = 0;

    load_dungeon_sprites();

    create_player();
    draw_dungeon();
}

void game_update(){
    //FntPrint("Babaji : %d", cnt++);
    FntPrint("Direction : %d Position : %d, %d\n", 
                    get_player()->direction, 
                    get_player()->position.vx, 
                    get_player()->position.vy
    );
    FntPrint("VSync : %d, FPS : %d\n", get_vsync_time(), get_fps());

    draw_player();
    player_input();
}