#include "../sprite.h"
#include "../cd.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgs.h>

/// @brief : array list of all sprites to render
Array_List* sprites_list = NULL;

/**
 * @brief initialize and allocate the sprite array list
 * 
 * @param max_length max number of sprite to render
 * @param chunk_size chunk size of the sprite array list (keep this high if you plan to add a lot of sprite)
 */
void sprite_list_init(unsigned short max_length, unsigned short chunk_size){
    sprites_list = array_list_create(max_length, chunk_size, sizeof(Sprite*));
}

/**
 * @brief initialize and allocate a new sprite in the array list, but his texture is'nt loaded yet
 * 
 * @param sprite_name name of the texture that the sprite will load when calling sprite_list_load
 * @return Sprite* pointer to the registered sprite
 */
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
    sprite->active = 1;
    
    sprite->childs_list = array_list_create(20, 2, sizeof(Sprite*));

    array_list_append(sprites_list, &sprite);

    /*
    printf("Address of sprite : %p\n", sprite);
    printf("Adress of array sprite : %p\n", (Sprite*) array_list_get(sprites_list, sprites_list->length-1));
    */

    //return (Sprite*) array_list_get(sprites_list, sprites_list->length-1);
    return sprite;
}

/**
 * @brief load all sprite's texture registered in list with sprite_register, this is meant to open and close the cd system once
 * 
 */
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

/**
 * @brief create all the texture data of the sprite (this is an init for Psy-Q's GsSprite contained inside my Sprite struct wrapper)
 * 
 * @param image_data image data of the sprite
 * @param sprite pointer to the sprite that will hold the image data
 */
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

/**
 * @brief get the sprite array list
 * 
 * @return Array_List* : sprite_list
 */
Array_List* get_sprite_list(){
    return sprites_list;
}

/**
 * @brief set sprite active or inactive, if active <= 0 the sprite won't be drawn
 * 
 * @param sprite pointer to the sprite to set active
 * @param active new value for active sprite's member
 */
void sprite_set_active(Sprite* sprite, unsigned short active){
    sprite->active = active;
}

/**
 * @brief set sprite z_index : 0 is front, the highest back value is calculated using OT_LENGTH (see sprite.h)
 * 
 * @param sprite pointer to the sprite to update
 * @param z_index new value for sprite's z_index
 */
void sprite_set_z_index(Sprite* sprite, unsigned short z_index){
    sprite->z_index = z_index;
}

/**
 * @brief get the sprite's scale
 * 
 * @param sprite pointer to the sprite
 * @return vector2 : sprite's scale
 */
vector2 sprite_get_scale(Sprite* sprite){
    vector2 scale = {
        .vx = sprite->sprite_data->scalex,
        .vy = sprite->sprite_data->scaley
    };

    return scale;
}

/**
 * @brief set the sprite's scale
 * 
 * @param sprite pointer to the sprite
 * @param scale new scale
 */
void sprite_set_scale_vector(Sprite* sprite, vector2 scale){
    sprite->sprite_data->scalex = scale.vx;
    sprite->sprite_data->scaley = scale.vy;
    //sprite_scale_pivot(sprite, scale);

    if(sprite->childs_list->length > 0){
        for(int c = 0; c < sprite->childs_list->length; c++){
            Sprite* child = *(Sprite**) array_list_get(sprite->childs_list, c);
            //vector2 new_pos = vector_add(&position, &child->parent_link->local_position);

            vector2 new_scale = vector2_cross_multiply((vector2*) &VECTOR_ONE, &child->parent_link->local_scale, &scale);
            sprite_set_scale_vector(child, new_scale);

            vector2 parent_pos = sprite_get_position_vector(sprite);
            //sprite_scale_pivot(child, new_scale);
            vector2 scaled_pos = vector2_cross_multiply((vector2*) &VECTOR_ONE, &child->parent_link->local_position, &scale);


            vector2 new_pos = vector2_add(&scaled_pos, &parent_pos);
            sprite_set_position_vector(child, new_pos);
        }
    }
}

void sprite_scale_pivot(Sprite* sprite, vector2 new_scale){
    if(sprite->sprite_data->mx > 0){
        int scaled_mx = (int) (sprite->sprite_data->w) * (int) (new_scale.vx);
        scaled_mx = scaled_mx / (int) (-ONE);
        sprite->sprite_data->mx = scaled_mx;
    }
    
    if(sprite->sprite_data->my > 0){
        int scaled_my = (int) (sprite->sprite_data->h) * (int) (new_scale.vy);
        scaled_my = scaled_my / (int) (-ONE);
        sprite->sprite_data->my = scaled_my;
    }
}

/**
 * @brief [DEPRECATED] flip the sprite horizontally, 0 = no flip, 1 = flip
 * 
 * @param sprite pointer to the sprite
 * @param flip flip value
 */
void sprite_flip_horizontal(Sprite* sprite, unsigned short flip){
    if(flip > 0){
        sprite->sprite_data->scalex = -ONE;
        //sprite->sprite_data->mx = sprite->sprite_data->w;
        sprite->sprite_data->x -= (short) (((int) sprite->sprite_data->w * (int) sprite->sprite_data->scalex) / (int) ONE);
        //sprite->sprite_data->x -= sprite->sprite_data->w;
    } else {
        sprite->sprite_data->scalex = ONE;
        //sprite->sprite_data->mx = 0;
    }
}

/**
 * @brief [DEPRECATED] flip the sprite vertically, 0 = no flip, 1 = flip
 * 
 * @param sprite pointer to the sprite
 * @param flip flip value
 */
void sprite_flip_vertical(Sprite* sprite, unsigned short flip){
    if(flip > 0){
        sprite->sprite_data->scaley = -ONE;
        //sprite->sprite_data->my = sprite->sprite_data->h;
        sprite->sprite_data->y += (short) (((int) sprite->sprite_data->h * (int) sprite->sprite_data->scaley) / (int) ONE);
        //sprite->sprite_data->y += sprite->sprite_data->h;
    } else {
        sprite->sprite_data->scaley = ONE;
        //sprite->sprite_data->my = 0;
    }
}

/**
 * @brief get the sprite's position
 * 
 * @param sprite pointer to the sprite
 * @return vector2 : sprite's position
 */
vector2 sprite_get_position_vector(Sprite* sprite){
    vector2 pos = { .vx = sprite->sprite_data->x, .vy = sprite->sprite_data->y};
    return pos;
}

/**
 * @brief set the sprite's position
 * 
 * @param sprite pointer to the sprite
 * @param position new position
 */
void sprite_set_position_vector(Sprite* sprite, vector2 position){
    sprite->sprite_data->x = position.vx;
    sprite->sprite_data->y = position.vy;

    if(sprite->childs_list->length > 0){
        for(int c = 0; c < sprite->childs_list->length; c++){
            Sprite* child = *(Sprite**) array_list_get(sprite->childs_list, c);

            vector2 parent_scale = sprite_get_scale(sprite);
            vector2 scaled_local_pos = vector2_cross_multiply((vector2*) &VECTOR_ONE, &child->parent_link->local_position, &parent_scale);
            vector2 new_pos = vector2_add(&position, &scaled_local_pos);

            sprite_set_position_vector(child, new_pos);
        }
    }
}

/**
 * @brief set the sprite's position using x and y instead of a vector2
 * 
 * @param sprite pointer to the sprite
 * @param x new x value
 * @param y new y value
 */
void sprite_set_position(Sprite* sprite, int x, int y){
    vector2 pos = { .vx = x, .vy = y };
    sprite_set_position_vector(sprite, pos);
}

/**
 * @brief set the sprite's x value
 * 
 * @param sprite pointer to the sprite
 * @param x new x value
 */
void sprite_set_x(Sprite *sprite, int x){
    sprite_set_position(sprite, x, sprite->sprite_data->y);
}

/**
 * @brief set the sprite's y value
 * 
 * @param sprite pointer to the sprite
 * @param y new y value
 */
void sprite_set_y(Sprite *sprite, int y){
    sprite_set_position(sprite, sprite->sprite_data->x, y);
}

/**
 * @brief 
 * 
 * @param sprite 
 * @param move 
 * @return vector2 
 */
vector2 sprite_move_vector(Sprite *sprite, vector2 move){
    vector2 sprite_position = sprite_get_position_vector(sprite);
    vector2 calculated_motion = vector2_add(
        &sprite_position,
        &move
    );
    sprite_set_position_vector(sprite, calculated_motion);
    return calculated_motion;
}

/**
 * @brief create a parent link between two sprites : the child will follow the position and scale of his parent, rotation is WIP
 * 
 * @param parent parent sprite
 * @param child child sprite
 */
void sprite_link(Sprite* parent, Sprite* child){
    vector2 child_position = sprite_get_position_vector(child);
    vector2 parent_position = sprite_get_position_vector(parent);

    vector2 local_position = vector2_substract(&child_position, &parent_position);

    vector2 parent_scale = sprite_get_scale(parent);
    vector2 child_scale = sprite_get_scale(child);


    SpriteLink* link = (SpriteLink*) malloc3(sizeof(SpriteLink)); 

    link->parent = parent;
    link->local_position = vector2_cross_multiply(&parent_scale, &local_position, (vector2*) &VECTOR_ONE);
    link->local_scale = vector2_cross_multiply(&parent_scale, &child_scale, (vector2*) &VECTOR_ONE);

    child->parent_link = link;
    array_list_append(parent->childs_list, &child);
}