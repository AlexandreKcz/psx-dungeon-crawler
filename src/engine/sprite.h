#pragma once

#include "./types/array_list.h"
#include "./types.h"

#include <libgs.h>

#define ROT_ONE 4096

typedef struct sprite_link SpriteLink;

typedef struct sprite {
    char sprite_name[32];
    GsSPRITE* sprite_data;
    unsigned short z_index; //WITH OT_LENGTH 9, 2^9 = 512 so 511 is the background
    unsigned short active;
    Array_List* childs_list;    //array list of sprite_link
    SpriteLink* parent_link;
} Sprite;

typedef struct sprite_link {
    Sprite* parent;
    vector2 local_position;
    vector2 local_scale;
} SpriteLink;

void sprites_list_init(unsigned short max_length, unsigned short chunk_size);
Sprite* sprite_register(unsigned char* sprite_name);
void sprite_list_load();
void sprite_create(unsigned long* image_data, Sprite* sprite);
Array_List* get_sprite_list();

void sprite_set_active(Sprite* sprite, unsigned short active);
void sprite_set_z_index(Sprite* sprite, unsigned short z_index);

vector2 sprite_get_scale(Sprite* sprite);
void sprite_set_scale_vector(Sprite* sprite, vector2 scale);


void sprite_flip_horizontal(Sprite* sprite, unsigned short flip);
void sprite_flip_vertical(Sprite* sprite, unsigned short flip);

vector2 sprite_get_position_vector(Sprite* sprite);
void sprite_set_position_vector(Sprite* sprite, vector2 position);
void sprite_set_position(Sprite* sprite, int x, int y);
void sprite_set_x(Sprite* sprite, int x);
void sprite_set_y(Sprite* sprite, int y);

vector2 sprite_move_vector(Sprite* sprite, vector2 move);
vector2 sprite_move(Sprite* sprite, int x, int y);
vector2 sprite_move_x(Sprite* sprite, int x);
vector2 sprite_move_y(Sprite* sprite, int y);

void sprite_link(Sprite* parent, Sprite* child);

vector2 _vector_cross_multiply(vector2* s1, vector2* s2, vector2* m);