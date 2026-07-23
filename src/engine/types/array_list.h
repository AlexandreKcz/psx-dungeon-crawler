#pragma once

typedef struct array_list {
    unsigned short length;          //current number of items
    unsigned short max_length;      //max number of items
    unsigned long item_size;        //allocated memory for on item
    void* items;                    //void pointer on items
} Array_List;

Array_List *array_list_create(unsigned short max_length, unsigned long item_size);
unsigned short array_list_append(Array_List* list, void* item);
unsigned short array_list_remove(Array_List* list, unsigned short index);
void* array_list_get(Array_List* list, unsigned short index);