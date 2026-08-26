#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./array_list.h"

///this code is heavily based on this array list implementation in C made by Dylan Falconer <https://github.com/Falconerd/engine-from-scratch/blob/rec/src/engine/array_list/array_list.c>

/*
    [TODO]
    - get element by value
    - contains method
    - swap elements
*/

//// boolean to enable debug logs for array lists
short log_array = 0;

/**
 * @brief create an array list, allocate and initialize it
 * 
 * @param max_length the maximum number of element allowed in the array to prevent overflow
 * @param chunk_size the size of an array's chunk : dynamic array are allocated by chunks in memory, when a chunk is full the array dynamically allocate another chunk
 * @param item_size the size of the type of element the array will contain : array list can only contain one type of element
 * @return Array_List* : pointer to the array list to initialize
 */
Array_List* array_list_create(unsigned short max_length, unsigned short chunk_size, unsigned long item_size){
    Array_List* list = malloc3(sizeof(Array_List));

    printf("Sizeof array list : %i\n", sizeof(Array_List));
    printf("Max length: %i\n", max_length);

    if(!list){
        printf("Could not allocate memory for Array_List\n");
        return NULL;
    }

    list->item_size = item_size;
    list->length = 0;
    list->max_length = max_length;
    list->chunk_size = chunk_size;
    list->items = malloc3(item_size * chunk_size);
    list->chunks = 1;

    if(!list->items){
        printf("Could not allocate memory for Array_List items\n");
        free3(list);
        return NULL;
    }

    return  list; //TODO : maybe do this with a double pointer in params instead for consistency in code ?
}

/**
 * @brief append an item to an initialised array list
 * 
 * @param list pointer to the list that will receive the item
 * @param item item to append to the array
 * @return unsigned short : index of the added item in the array
 */
unsigned short array_list_append(Array_List* list, void* item){
    if(log_array > 0) printf("Appending to array list\n");
    if(!list || !list->items){
        printf("List is not initialized");
        return 0;
    }

    if(list->length >= list->max_length){
        printf("Max Length %i reached for Array List\n", list->max_length);
        return 0;
    }

    if(list->length >= (unsigned int) list->chunks * list->chunk_size){
        list->chunks++;
        void* realloc_items = realloc3(list->items, list->item_size * list->chunks * list->chunk_size);

        if(!realloc_items){
            printf("Out of memory for chunk reallocation on Array List\n");
            list->chunks--;
            return 0;
        }
        list->items = realloc_items;
        if(log_array > 0) printf("Allocating new chunk for array list\n");
    }

    unsigned short index = list->length;
    unsigned char* dest = (unsigned char*) list->items + (index * list->item_size);
    memcpy(dest, item, list->item_size);

    list->length++;

    return index;
}

/**
 * @brief quickly remove an item out of the array and replace it with the last element of the array to free memory without reallocating
 * 
 * @param list pointer to the list that will have the element removed
 * @param index index of the element to remove
 * @return unsigned short : 1 if the element has been removed, 0 if it failed
 */
unsigned short array_list_fast_remove(Array_List* list, unsigned short index){
    if(list->length == 0){
        printf("List is empty\n");
        return 0;
    }

    if(index >= list->length){
        printf("Index out of bounds\n");
        return 0;
    }

    if(list->chunks > 1 && list->length <= (list->chunks - 1) * list->chunk_size) {
        list->chunks--;
        list->items = realloc3(list->items, list->item_size * list->chunks * list->chunk_size);
    }

    --list->length;

    unsigned char* item_ptr = (unsigned char*) list->items + index * list->item_size;
    unsigned char* end_ptr  = (unsigned char*) list->items + list->length * list->item_size;
    
    if (index != list->length)
        memcpy(item_ptr, end_ptr, list->item_size);

    return 1;
}

/**
 * @brief remove an item out of the array while keeping element's order, this is longer than quick remove because it uses memory reallocation, but it keeps element in order
 * 
 * @param list pointer to the list that will have the element removed
 * @param index index of the element to remove
 * @return unsigned short : 1 if the element has been removed, 0 if it failed
 */
unsigned short array_list_order_remove(Array_List* list, unsigned short index){
        if(list->length == 0){
        printf("List is empty\n");
        return 0;
    }

    if(index >= list->length){
        printf("Index out of bounds\n");
        return 0;
    }

    if(list->chunks > 1 && list->length <= (list->chunks - 1) * list->chunk_size) {
        list->chunks--;
        list->items = realloc3(list->items, list->item_size * list->chunks * list->chunk_size);
    }

    if(index < list->length - 1){
        unsigned char* dst = (unsigned char*) list->items + (index * list->item_size);
        unsigned char* src = (unsigned char*) list->items + ((index + 1) * list->item_size);
        unsigned int bytes_to_move = (list->length - 1 - index) * list->item_size;

        memmove(dst, src, bytes_to_move);
    }

    --list->length;

    return 1;
}

/**
 * @brief get an element inside an array
 * 
 * @param list pointer to the list that will query the element
 * @param index index of the element to query
 * @return void* void pointer to the element queried, require casting to the correct type of the element
 */
void* array_list_get(Array_List* list, unsigned short index){
    if (index >= list->length){
        printf("Index out of bounds\n");
        return NULL;
    }

    return (unsigned char*) list->items + index * list->item_size;
}

/**
 * @brief free an array list in memory
 * 
 * @param list pointer to the array list to free
 */
void array_list_free(Array_List* list){
    if(list){
        if(list->items)
            free3(list->items);
        free3(list);
    }
}