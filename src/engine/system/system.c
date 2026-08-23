#include "../system.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief initialize memory heap, comes from Wituz's Ps1 tutorial : <https://github.com/Wituz/wituz-youtube/blob/master/ps1-game-tutorial/PART%209%20-%20CD%20Loading/project/constants.h>
 * 
 */
void _initialize_memory_heap(){
    printf("\nReserving 1024KB (1,048,576 Bytes) RAM... \n");
    InitHeap3((void*)0x800F8000, 0x00100000);
    printf("Success!\n");
}