#ifndef _MAP_INCLUDED
#define _MAP_INCLUDED

#define REGION_SIZE 2
#define TILE_SIZE 2048

#include "util.h"

typedef struct {
    list_t objs;
} tile_t;

typedef struct {
    int x, y;
    tile_t* tiles[REGION_SIZE][REGION_SIZE];
} region_t;

tile_t* get_tile(int x, int y);
tile_t* get_player_tile(player_info_t* player);

#include "map.c"

#endif // _MAP_INCLUDED
