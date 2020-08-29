#include <stdlib.h>

#include "util.h"

#define MAX_CLOUDS_PER_TILE 50
#define MAX_CLOUD_SIZE 40

tile_t* p_tile;

void add_random_points(tile_t* tile) {
    for (int i = 0; i < rand() % MAX_CLOUDS_PER_TILE; i++) {
        val_point_t* new = malloc(sizeof(val_point_t));
        new->x = rand() % TILE_SIZE;
        new->y = rand() % TILE_SIZE;
        new->val = rand() % MAX_CLOUD_SIZE + 10;
        push_to_list(&tile->objs, new); 
    }
}

tile_t* new_tile() {
    tile_t* new = malloc(sizeof(tile_t));
    for (int i = 0; i < 4; i++) {
        new->adjs[i] = NULL;
    }
    new->objs = new_list();
    add_random_points(new);
    return new;
}

void traverse(int dir) {
    if (p_tile->adjs[dir]) {
        p_tile = p_tile->adjs[dir];
    }
    else {
        tile_t* new = new_tile();
        p_tile->adjs[dir] = new;

        if (dir % 2)
            new->adjs[dir - 1] = p_tile;
        else
            new->adjs[dir + 1] = p_tile;

        p_tile = new;
    }
}
