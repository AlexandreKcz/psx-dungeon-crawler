#pragma once

#include "./types/array_list.h"

#include <libgs.h>

#define ROT_ONE 4096

typedef struct sprite {
    char sprite_name[32];
    GsSPRITE* sprite_data;
    unsigned short z_index; //WITH OT_LENGTH 9, 2^9 = 512 so 511 is the background
    unsigned short active;
} Sprite;

void sprites_list_init(unsigned short max_length, unsigned short chunk_size);
Sprite* sprite_register(unsigned char* sprite_name);
void sprite_list_load();
void sprite_create(unsigned long* image_data, Sprite* sprite);
Array_List* get_sprite_list();

void sprite_flip_horizontal(Sprite* sprite, unsigned short flip);
void sprite_flip_vertical(Sprite* sprite, unsigned short flip);