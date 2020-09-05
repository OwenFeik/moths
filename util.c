#include <stdlib.h>

#include "util.h"

list_t new_list(void) {
    list_t list;

    list.base = NULL;
    list.head = NULL;
    list.length = 0;

    return list;
}

void push_to_list(list_t* list, void* obj) {
    list_node_t* new = malloc(sizeof(list_node_t));
    new->next = NULL;
    new->obj = obj;

    if (list->head) {
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
    list_node_t* base = list->base;
    list->base = base->next;
    void* obj = base->obj;
    free(base);
    list->length -= 1;
    return obj;
}

void for_each(list_t* list, void (*func)(void*)) {
    if (!list->base)
        return;
  
    list_node_t* node = list->base;
    while (node) {
        func(node->obj);
        node = node->next;
    }
}

dlist_t new_dlist(void) {
    dlist_t list;

    list.base = NULL;
    list.head = NULL;
    list.length = 0;

    return list;
}

void push_to_dlist(dlist_t* list, void* obj) {
    dlist_node_t* new = malloc(sizeof(dlist_node_t));
    new->prev = list->head;
    new->next = NULL;
    new->obj = obj;

    if (list->head) {
        list->head->next = new;
        list->head = new;
    }
    else {
        list->base = new;
        list->head = new;
    }
    
    list->length += 1;
}

void* pop_from_dlist(dlist_t* list) {
    dlist_node_t* base = list->base;
    
    if (base == list->head) {
        list->base = NULL;
        list->head = NULL;
    }
    else {
        list->base = base->next;
        list->base->prev = NULL;
    }

    void* obj = base->obj;
    free(base);
    list->length -= 1;
    return obj;
}

void remove_from_dlist(dlist_t* list, dlist_node_t* node) {
    if (node->next && node->prev) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    free(node);
    free(node->obj);
}
