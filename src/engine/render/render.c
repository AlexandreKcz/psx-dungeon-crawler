#include "../render.h"
#include "render_internal.h"

#include <libetc.h>
#include <stdio.h>
#include <libgs.h>
#include <libgpu.h>

#include "../prims.h"
#include "../types/array_list.h"
#include "../sprite.h"

/// a lot of code here come's from Wituz's Ps1 Development tutorial : <https://github.com/Wituz/wituz-youtube/blob/master/ps1-game-tutorial/PART%209%20-%20CD%20Loading/project/constants.h>

/// screen dimension
int screen_width, screen_height;

/// @brief main ordering tables
GsOT        ordering_tables[2];
/// @brief minor ordering tables
GsOT_TAG    minor_ordering_tables[2][1<<OT_LENGTH];
/// @brief packet buffer for GPU commands
PACKET gpu_output_packet[2][PACKETMAX];
/// @brief short to keep track of current buffer for double buffering
short       current_buffer;

/// @brief color pointer to background_color
Color*      background_color;

/// @brief Array list containing all lines primitives to display
Array_List* lines_list;

/**
 * @brief initialize screen with Psy-Q's function : set drawing zones, init background color, ordering tables and primitives array list
 * 
 */
void _initialize_screen() {
    if (*(char *)0xbfc7ff52 == 'E')
        set_screen_mode(SCREEN_MODE_PAL);
    else
        set_screen_mode(SCREEN_MODE_NTSC);

    SetDispMask(1);
    ResetGraph(0);
    _clear_vram();

    GsInitGraph(screen_width, screen_height, GsINTER|GsOFSGPU, 1, 0);
    GsDefDispBuff(0, 0, 0, screen_height);

    _initialize_ordering_table();
    color_create(0, 0, 0, &background_color);

    lines_list = array_list_create(500, 5, sizeof(Line*));
    printf("array list max length : %i \n",lines_list->max_length);
}

/**
 * @brief set the screen mode to PAL or NTSC (see : render_internal.h for regional screen size constants)
 * 
 * @param mode use SCREEN_MODE_PAL or SCREEN_MODE_NTSC from render_internal.h
 */
void set_screen_mode(int mode) {
    screen_width = SCREEN_WIDTH_COMMON;

    if(mode == SCREEN_MODE_PAL) {
        screen_height = SCREEN_HEIGHT_PAL;
        printf("Setting video mode to PAL \n");
        SetVideoMode(1);
    } else {
        screen_height = SCREEN_HEIGHT_NTSC;
        printf("Setting video mode to NTSC \n");
        SetVideoMode(0);
    }

    printf("Screen resolution : %d x %d \n", screen_width, screen_height);
    printf("Playstation video mode : %d \n", GetVideoMode());

    GsInitGraph(screen_width, screen_height, GsINTER|GsOFSGPU, 1, 0);
    GsDefDispBuff(0, 0, 0, screen_height);
}


/**
 * @brief clear the entire video ram and wait for vertical sync
 * 
 */
void _clear_vram(){
    RECT rectTL;
    setRECT(&rectTL, 0, 0, 1024, 512);
    ClearImage2(&rectTL, 0, 0, 0);
    DrawSync(0);
}

/**
 * @brief initialize ordering tables using OT_LENGTH const from render_internal.h
 * 
 */
void _initialize_ordering_table(){
    GsClearOt(0, 0, &ordering_tables[GsGetActiveBuff()]);

    ordering_tables[0].length = OT_LENGTH;
    ordering_tables[1].length = OT_LENGTH;
    ordering_tables[0].org = minor_ordering_tables[0];
    ordering_tables[1].org = minor_ordering_tables[1];

    GsClearOt(0, 0, &ordering_tables[0]);
    GsClearOt(0, 0, &ordering_tables[1]);
}

/**
 * @brief initialize Psy-Q's default debug font (can be overriden at vram coordinates : 960, 256)
 * 
 */
void initialize_debug_font() {
    FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512));
}

/**
 * @brief register line in prim's array list for rendering
 * 
 * @param line pointer to the line to register
 */
void line_register(Line* line) {

    array_list_append(lines_list, &line);

    /*
    if(crntLinesNumber + 1 > PRIMS_ARRAY_SIZE - 1){
        printf("Cannot register anymore lines, increase PRIMS_ARRAY_SIZE in render_internal.h to more than : %d\n", PRIMS_ARRAY_SIZE);
        return;
    }

    *(linesToDraw + crntLinesNumber++) = line;
    printf("Registering new line in linesToDraw currently : %d \n", crntLinesNumber);
    */
}

/**
 * @brief handle update loop for renderer : clear display, call draw then display
 * 
 */
void _render_update() {
    clear_display();
    _draw();
    _display();
}

/**
 * @brief get current buffer, change gpu_packet with current buffer and clear the buffer's ordering table
 * 
 */
void clear_display() {
    current_buffer = GsGetActiveBuff();
    GsSetWorkBase((PACKET*)gpu_output_packet[current_buffer]);
    GsClearOt(0, 0, &ordering_tables[current_buffer]);
}

/**
 * @brief handle renderer's draw function : automatically draw every prims inside lines_list array and every sprite inside sprite_list_array
 * 
 */
void _draw() {
    current_buffer = GsGetActiveBuff();
    if(get_sprite_list() != NULL){
        for(int i = 0; i < get_sprite_list()->length; i++){
            Sprite* sprite = *(Sprite**) array_list_get(get_sprite_list(), i);
            if(sprite->active > 0){
                GsSortSprite(sprite->sprite_data, &ordering_tables[current_buffer], sprite->z_index);
            }
        }
    }

    for(int i = 0; i < lines_list->length; i++){
        Line* line = *(Line**) array_list_get(lines_list, i); //TODO : maybe move the lines_list to prims.c
        line_draw(
            line
        );
    }
}

/**
 * @brief wait for vsync, switch current buffer for double buffering, clear ordering table and current font
 * 
 */
void _display() {
    current_buffer = GsGetActiveBuff();
    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    GsSortClear(background_color->r, background_color->g, background_color->b, &ordering_tables[current_buffer]);
    GsDrawOt(&ordering_tables[current_buffer]);
    FntFlush(-1);
}