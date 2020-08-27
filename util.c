#include <stdlib.h>
#include <stdio.h>

#include "util.h"

list_t new_list(void) {
    list_t list;

    list.base = NULL;
    list.head = NULL;
    list.length = 0;

    return list;
}

void push_to_list(list_t* list, void* obj) {
    struct list_node* new = malloc(sizeof(struct list_node));
    new->next = NULL;
    new->obj = obj;

    if (list-> head) {
        list->head->next = new;
        list->head = new;
    }
    else {
        list->base = new;
        list->head = new;
    }
    
    list->length += 1;
}

void* pop_from_list(list_t* list) {
    struct list_node* base = list->base;
    list->base = base->next;
    void* obj = base->obj;
    free(base);
    return obj;
}

void for_each(list_t* list, void (*func)(void*)) {
    if (!list->base)
        return;
  
    struct list_node* node = list->base;
    while (node) {
        func(node->obj);
        node = node->next;
    }
}
