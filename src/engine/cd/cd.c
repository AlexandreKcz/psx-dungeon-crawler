#include "../cd.h"

#include <libds.h>

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