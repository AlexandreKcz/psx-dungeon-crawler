#include "../model.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Array_List* model_list = NULL;

void model_list_init(unsigned short max_length, unsigned short chunk_size){
    model_list = array_list_create(max_length, chunk_size, sizeof(Model));
}

Model* model_register(unsigned char* model_name){
    if(!model_list){
        printf("WARNING : sprites_list has not been initialized, initializing with default values : max_length = 500, chunk_size = 5\n");
        model_list_init(500, 5);
    }

    Model* model = (Model*) malloc3(sizeof(Model));
    strncpy(model->model_name, model_name, 31);
    model->model_name[31] = '\0';
    model->model_data = NULL;
    model->active = 1;
    model->affected_by_light = 1;

    //TODO : handle parenting

    array_list_append(model_list, &model);

    return model;
}

void model_list_load(){
    
}