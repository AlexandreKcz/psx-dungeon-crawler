#include "../cd.h"

#include <libds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../types/array_list.h"

int ds_init = 0;

void cd_open(){
    if(ds_init == 0){
        ds_init = 1;
        DsInit();
    }
}

void cd_close(){
    if(ds_init == 1){
        ds_init = 0;
        DsClose();
    }
}

short cd_is_open(){
    return ds_init;
}

void cd_read_file(unsigned char* file_path, unsigned long** file){

    DslFILE temp_file_info;
    char file_path_raw[64];

    if(cd_is_open() == 0){
        printf("Should call cd_open() before reading form cd \n");
        return;
    }

    sprintf(file_path_raw, "\\%s;1", file_path);
    printf("Loading file from CD: %s\n", file_path_raw);

    DsSearchFile(&temp_file_info, file_path_raw);

    if(temp_file_info.size > 0){
        printf("File : %s found !\n", file_path_raw);
        
        int num_sectors = (temp_file_info.size + SECTOR - 1) / SECTOR;
        int buffer_size = num_sectors * SECTOR;

        *file = (unsigned long*) malloc3(buffer_size);

        if(*file == NULL) {
            printf("Could not allocate RAM for file buffer\n");
            return;
        }

        DsRead(&temp_file_info.pos, num_sectors, *file, DslModeSpeed);
        while(DsReadSync(NULL));

        printf("File : %s loaded !\n", file_path_raw);

    } else {
        printf("File : %s not found \n", file_path_raw);
        *file = NULL;
    }
}