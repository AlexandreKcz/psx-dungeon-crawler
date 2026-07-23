#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./array_list.h"

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

    return  list;
}

unsigned short array_list_append(Array_List* list, void* item){
    printf("Appending to array list\n");
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
        printf("Allocating new chunk for array list\n");
    }

    unsigned short index = list->length;
    unsigned char* dest = (unsigned char*) list->items + (index * list->item_size);
    memcpy(dest, item, list->item_size);

    list->length++;

    return index;
}

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

void* array_list_get(Array_List* list, unsigned short index){
    if (index >= list->length){
        printf("Index out of bounds\n");
        return NULL;
    }

    return (unsigned char*) list->items + index * list->item_size;
}

void array_list_free(Array_List* list){
    if(list){
        if(list->items)
            free3(list->items);
        free3(list);
    }
}