#pragma once

#include "types.h"
#include "./types/array_list.h"

#include <libgs.h>

typedef struct model {
    char model_name[32];
    char model_texture[32];
    GsDOBJ2* model_data;
    vector3 position;
    vector3 rotation;
    unsigned short active;
    unsigned short affected_by_light;
    //TODO : parenting
} Model;

void model_list_init(unsigned short max_length, unsigned short chunk_size);
Model* model_register(unsigned char* model_name, unsigned char* model_texture);
void model_list_load();
void model_create(unsigned long* image_data, Model* model);
Array_List* get_model_list();

void load_TMD(unsigned long* tmd, GsDOBJ2* obj);
void load_texture(unsigned char* image_data);