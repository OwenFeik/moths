#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED

#define TICK_RATE 60

#define EPSILON 0.00001

#define PI 3.141593
#define TAU 6.283185

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct list_node {
    struct list_node* next;
    void* obj;
};

typedef struct {
    struct list_node* base;
    struct list_node* head;
    int length;
} list_t;

list_t new_list(void);
void push_to_list(list_t* list, void* obj);
void* pop_from_list(list_t* list);
void for_each(list_t* list, void (*func)(void*));

#endif // _UTIL_INCLUDED
