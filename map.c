#include <stdlib.h>

#define MAX_CLOUDS_PER_TILE 50
#define MAX_CLOUD_SIZE 40

list_t regions;
region_t* region;
int p_tile_x, p_tile_y;

void add_random_points(tile_t* tile) {
    for (int i = 0; i < rand() % MAX_CLOUDS_PER_TILE; i++) {
        val_point_t* new = malloc(sizeof(val_point_t));
        new->x = rand() % TILE_SIZE;
        new->y = rand() % TILE_SIZE;
        new->val = rand() % MAX_CLOUD_SIZE + 10;
        push_to_list(&tile->objs, new); 
    }
}

tile_t* new_tile(void) {
    tile_t* new = malloc(sizeof(tile_t));
    new->objs = new_list();
    add_random_points(new);
    return new;
}

region_t* new_region(int x, int y) {
    region_t* new = malloc(sizeof(region_t));
    new->x = x;
    new->y = y;
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            new->tiles[i][j] = new_tile();
        }
    }
    return new;
}

region_t* get_region(int x, int y) {
    list_node_t* node = regions.base;
    while (node) {
        region_t* reg = (region_t*) node->obj;
        if (reg->x == x && reg->y == y) {
            return node->obj;
        }
        node = node->next;
    }

    region_t* new = new_region(x, y);
    push_to_list(&regions, new);
    return new;
}

tile_t* get_tile(int x, int y) {
    int dx = 0;
    int dy = 0;

    if (!(x < REGION_SIZE)) {
        dx += 1;
        x = 0;
    }
    else if (x < 0) {
        dx -= 1;
        x = REGION_SIZE - 1;
    }

    if (!(y < REGION_SIZE)) {
        dy += 1;
        y = 0;
    }
    else if (y < 0) {
        dy -= 1;
        y = REGION_SIZE - 1;
    }

    if (dx == 0 && dy == 0)
        return region->tiles[y][x];
    else
        return get_region(region->x + dx, region->y + dy)->tiles[y][x];
}

tile_t* get_player_tile(void) {
    return get_tile(p_tile_x, p_tile_y);
}

void traverse_region(int dx, int dy) {
    region = get_region(region->x + dx, region->y + dy);
}

void traverse_tile(int dx, int dy) {
    p_tile_x += dx;
    p_tile_y += dy;

    if (!(p_tile_x < REGION_SIZE)) {
        traverse_region(1, 0);
        p_tile_x = 0;
    }
    else if (p_tile_x < 0) {
        traverse_region(-1, 0);
        p_tile_x = REGION_SIZE - 1;
    }

    if (!(p_tile_y < REGION_SIZE)) {
        traverse_region(0, 1);
        p_tile_y = 0;
    }
    else if (p_tile_y < 0) {
        traverse_region(0, -1);
        p_tile_y = REGION_SIZE - 1;
    }
}

void init_map(void) {
    regions = new_list();
    region = new_region(0, 0);
    push_to_list(&regions, region);
    p_tile_x = p_tile_y = 0;
}
