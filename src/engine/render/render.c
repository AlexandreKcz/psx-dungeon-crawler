#include "../render.h"
#include "render_internal.h"

#include <libetc.h>
#include <stdio.h>
#include <libgs.h>
#include <libgpu.h>

#include "../prims/prims.h"
#include "../types/array_list.h"
#include "../sprite.h"

int screen_width, screen_height;

GsOT        orderingTables[2];
GsOT_TAG    minorOrderingTable[2][1<<OT_LENGTH];
PACKET GPUOutputPacket[2][PACKETMAX];
short       currentBuffer;

Color*      backgroundColor;

Array_List* lines_list;

/*
Line* linesToDraw[PRIMS_ARRAY_SIZE];
int crntLinesNumber = 0;
*/

void initialize_screen() {
    if (*(char *)0xbfc7ff52 == 'E')
        set_screen_mode(SCREEN_MODE_PAL);
    else
        set_screen_mode(SCREEN_MODE_NTSC);

    SetDispMask(1);
    ResetGraph(0);
    clear_vram();

    GsInitGraph(screen_width, screen_height, GsINTER|GsOFSGPU, 1, 0);
    GsDefDispBuff(0, 0, 0, screen_height);

    /*
    GsSetOffset(screen_width / 2, screen_height / 2);
    GsInitCoordinate2(0, 0);
    */

    initialize_oredering_table();
    color_create(0, 0, 0, &backgroundColor);

    lines_list = array_list_create(500, 5, sizeof(Line*));
    printf("array list max length : %i \n",lines_list->max_length);

    /*
    for(int i = 0; i < (sizeof(linesToDraw) / sizeof(linesToDraw[0])); i++) {
        *(linesToDraw + i) = NULL;
    }

    crntLinesNumber = 0;
    */
}

void set_screen_mode(int mode) {
    screen_width = 320;

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

void clear_vram(){
    RECT rectTL;
    setRECT(&rectTL, 0, 0, 1024, 512);
    ClearImage2(&rectTL, 0, 0, 0);
    DrawSync(0);
}

void initialize_oredering_table(){
    GsClearOt(0, 0, &orderingTables[GsGetActiveBuff()]);

    orderingTables[0].length = OT_LENGTH;
    orderingTables[1].length = OT_LENGTH;
    orderingTables[0].org = minorOrderingTable[0];
    orderingTables[1].org = minorOrderingTable[1];

    GsClearOt(0, 0, &orderingTables[0]);
    GsClearOt(0, 0, &orderingTables[1]);
}

void initialize_debug_font() {
    FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512));
}

void line_register(Line* line) {

    //printf("Registering new line\n");

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

void render_update() {
    clear_display();
    draw();
    display();
}

void clear_display() {
    currentBuffer = GsGetActiveBuff();
    GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
    GsClearOt(0, 0, &orderingTables[currentBuffer]);
}

void draw() {
    currentBuffer = GsGetActiveBuff();

    if(get_sprite_list() != NULL){
        for(int i = 0; i < get_sprite_list()->length; i++){
            Sprite* sprite = array_list_get(get_sprite_list(), i);
            //printf("Drawing sprite : %s\n", sprite->sprite_name);
            GsSortSprite(sprite->sprite_data, &orderingTables[currentBuffer], 0);
        }
    }

    for(int i = 0; i < lines_list->length; i++){
        Line* line = *(Line**) array_list_get(lines_list, i);
        line_draw(
            line
        );
    }


    /*
    for(int i = 0; i < (sizeof(linesToDraw) / sizeof(linesToDraw[0])); i++) {
        line_draw(*(linesToDraw + i));
    }
    */

    //sprite draw here
}

void display() {
    currentBuffer = GsGetActiveBuff();
    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    GsSortClear(backgroundColor->r, backgroundColor->g, backgroundColor->b, &orderingTables[currentBuffer]);
    GsDrawOt(&orderingTables[currentBuffer]);
    FntFlush(-1);
}