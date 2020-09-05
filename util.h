#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED

#define TICK_RATE 60

#define EPSILON 0.000001

#define PI 3.141593
#define TAU 6.283185

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct list_node {
    struct list_node* next;
    void* obj;
} list_node_t;

typedef struct {
    list_node_t* base;
    list_node_t* head;
    int length;
} list_t;

typedef struct dlist_node {
    struct dlist_node* prev;
    struct dlist_node* next;
    void* obj;
} dlist_node_t;

typedef struct {
    dlist_node_t* base;
    dlist_node_t* head;
    int length;
} dlist_t;

enum directions {
    LEFT = 1,
    RIGHT = -1,
    FORWARD = 1,
    BACKWARD = -1
};

typedef struct {
    float x;
    float y;
    int val;
} val_point_t;

typedef struct {
    float x;
    float y;
    float v_x;
    float v_y;
    int tile_x;
    int tile_y;
    float dir;
} player_info_t;

list_t new_list(void);
void push_to_list(list_t* list, void* obj);
void* pop_from_list(list_t* list);
void for_each(list_t* list, void (*func)(void*));

dlist_t new_dlist(void);
void push_to_dlist(dlist_t* list, void* obj);
void* pop_from_dlist(dlist_t* list);
void remove_from_dlist(dlist_t* list, dlist_node_t* node);

#include "util.c"

#endif // _UTIL_INCLUDED
