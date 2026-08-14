#include "../sprite.h"
#include "../cd.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Array_List* sprites_list = NULL;

void sprite_list_init(unsigned short max_length, unsigned short chunk_size){
    sprites_list = array_list_create(max_length, chunk_size, sizeof(Sprite*));
}

Sprite* sprite_register(unsigned char* sprite_name){
    if(!sprites_list){
        printf("WARNING : sprites_list has not been initialized, initializing with default values : max_length = 500, chunk_size = 5\n");
        sprite_list_init(500, 5);
    }

    Sprite* sprite = (Sprite*) malloc3(sizeof(Sprite));
    strncpy(sprite->sprite_name, sprite_name, 31);
    sprite->sprite_name[31] = '\0';
    sprite->sprite_data = NULL;
    sprite->z_index = 0;
    sprite->flip = 0;
    sprite->active = 1;

    array_list_append(sprites_list, &sprite);

    /*
    printf("Address of sprite : %p\n", sprite);
    printf("Adress of array sprite : %p\n", (Sprite*) array_list_get(sprites_list, sprites_list->length-1));
    */

    //return (Sprite*) array_list_get(sprites_list, sprites_list->length-1);
    return sprite;
}

void sprite_list_load(){

    cd_open();

    for(int i = 0; i < sprites_list->length; i++){
        unsigned long* file_buffer = NULL;

        Sprite* sprite = *(Sprite**) array_list_get(sprites_list, i);

        cd_read_file(sprite->sprite_name, &file_buffer);

        if(file_buffer != NULL){
            sprite_create(file_buffer, sprite);
            printf("Sprite : %s created succesfully\n", sprite->sprite_name);
            free3(file_buffer);
        } else {
            printf("Error could not allocate sprite %s\n", sprite->sprite_name);
        }
    }

    cd_close();
}

void sprite_create(unsigned long* image_data, Sprite* sprite){
    GsIMAGE tim_data;
    RECT rect, crect;

    GsGetTimInfo(image_data + 1, &tim_data);

    rect.x = tim_data.px;
    rect.y = tim_data.py;
    rect.w = tim_data.pw;
    rect.h = tim_data.ph;
    LoadImage(&rect, tim_data.pixel);

    crect.x = tim_data.cx;
    crect.y = tim_data.cy;
    crect.w = tim_data.cw;
    crect.h = tim_data.ch;
    LoadImage(&crect, tim_data.clut);

    GsSPRITE* gs_sprite = (GsSPRITE*) malloc3(sizeof(GsSPRITE));

    //gs_sprite->attribute = 0x0000000;
    gs_sprite->attribute = 0;
    gs_sprite->x = 0;
    gs_sprite->y = 0;
    gs_sprite->w = tim_data.pw * 4;
    gs_sprite->h = tim_data.ph;

    gs_sprite->tpage = GetTPage(
			tim_data.pmode & 0x3, 		 // 0=4-bit, 1=8-bit, 2=16-bit
			1,       // semitransparency rate
			rect.x, // framebuffer x position of image
			rect.y  // framebuffer y position of image
	);
    
    gs_sprite->r = 128;
    gs_sprite->g = 128;
    gs_sprite->b = 128;
    gs_sprite->u = (tim_data.px * 4) % 256;
    gs_sprite->v = (tim_data.py % 256);
    gs_sprite->cx = tim_data.cx;
    gs_sprite->cy = tim_data.cy;
    gs_sprite->rotate = 0;
    gs_sprite->mx = 0;
    gs_sprite->my = 0;
    gs_sprite->scalex = ONE;
    gs_sprite->scaley = ONE;

    sprite->sprite_data = gs_sprite;
    //sprite->z_index = 0;
    //sprite->active = 1;

}

Array_List* get_sprite_list(){
    return sprites_list;
}