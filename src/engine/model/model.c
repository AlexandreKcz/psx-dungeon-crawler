#include "../model.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../cd.h"

Array_List* model_list = NULL;

void model_list_init(unsigned short max_length, unsigned short chunk_size){
    model_list = array_list_create(max_length, chunk_size, sizeof(Model));
}

Model* model_register(unsigned char* model_name, unsigned char* model_texture){
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
    cd_open();

    for(int i = 0; i < model_list->length; i++){
        unsigned long* tmd_buffer = NULL;
        unsigned long* tim_buffer = NULL;

        Model* model = *(Model**) array_list_get(model_list, i);

        cd_read_file(model->model_name, &tmd_buffer);
        cd_read_file(model->model_texture, &tim_buffer);

        load_texture((unsigned char*)tim_buffer);
        load_TMD(tmd_buffer, tim_buffer);
    }

    cd_close();
}

void load_TMD(unsigned long *tmd, GsDOBJ2 *obj){
    //TODO : clean this mess

    unsigned long* dop;
    int object_number;

    dop = tmd;
    dop++;
    GsMapModelingData(dop);

    dop++;
    object_number = *dop;

    dop++;
    GsMapModelingData(dop);

    dop++;
    object_number = *dop;

    dop++;

    for(int i = 0; i < object_number; i++){
        GsLinkObject4((unsigned long) dop, &obj[i], i);
    }
}

void load_texture(unsigned char* image_data){
    GsIMAGE* tim_data;
    RECT* rect;
    RECT* crect;
    tim_data = malloc3(sizeof(GsIMAGE));
    GsGetTimInfo((unsigned long*)(image_data+4), tim_data);
    rect = malloc3(sizeof(RECT));
    crect = malloc3(sizeof(RECT));

    rect->x = tim_data->px;
    rect->y = tim_data->py;
    rect->w = tim_data->pw;
    rect->h = tim_data->ph;
    LoadImage(rect, tim_data->pixel);

    crect->x = tim_data->cx;
    crect->y = tim_data->cy;
    crect->w = tim_data->cw;
    crect->h = tim_data->ch;
    LoadImage(crect, tim_data->clut);

    free3(rect);
    free3(crect);
    free3(tim_data);
}