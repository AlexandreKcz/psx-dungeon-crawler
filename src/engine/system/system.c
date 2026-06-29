#include "system.h"

#include <stdio.h>
#include <stdlib.h>

void initialize_memory_heap(){
    printf("\nReserving 1024KB (1,048,576 Bytes) RAM... \n");
    InitHeap3((void*)0x800F8000, 0x00100000);
    printf("Success!\n");
}