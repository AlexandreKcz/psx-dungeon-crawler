#include "../cd.h"

#include <libds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief boolean to check if Psy-Q's CD system has been initialized
int ds_init = 0;

/**
 * @brief Initialize CD System (if it has not been already initialized), wrapper for Psy-Q's DsInit function
 * 
 */
void cd_open(){
    if(ds_init == 0){
        ds_init = 1;
        DsInit();
    }
}

/**
 * @brief Clode CD System (if it is opened), wrapper for Psy-Q's DsClose function
 * 
 */
void cd_close(){
    if(ds_init == 1){
        ds_init = 0;
        DsClose();
    }
}

/**
 * @brief getter function for ds_init
 * 
 * @return short : boolean to check if Psy-Q's CD system has been initialized
 */
short cd_is_open(){
    return ds_init;
}

/**
 * @brief read file on the disk
 * 
 * @param file_path path of file on the disk file system
 * @param file double pointer to the file that will hold the data of the file read on disk
 */
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