#include <math.h>
#include <stdio.h>

#include "map.h"
#include "util.h"

#define DTHETA 0.05
#define DVEL 3

#define DRAG 0.5
#define MIN_DRAG 0.1
#define VELMAX 10

float to_range(float val, float geq, float leq) {
    if (val >= leq)
        return leq;
    if (val <= geq)
        return geq;
    return val;
}

void apply_rotation(player_info_t* player, float dir) {
    player->dir += DTHETA * dir;
    if (player->dir > TAU)
        player->dir -= TAU;
    else if (player->dir < -TAU)
        player->dir += TAU;
}

void apply_acceleration(player_info_t* player, int dir) {
    player->v_x += DVEL * dir * cos(player->dir);
    player->v_y += DVEL * dir * sin(player->dir);
}

void apply_drag(player_info_t* player) {
    int s_x = player->v_x > 0;
    int s_y = player->v_y > 0;

    double v_theta;
    double a_v_x = fabs(player->v_x);
    double a_v_y = fabs(player->v_y);

    if (a_v_x < EPSILON && a_v_y > EPSILON) {
        v_theta = (PI / 2) * (s_y * 1 + !s_y * -1);
        player->v_x = 0;
    }
    else if (a_v_y < EPSILON && a_v_x > EPSILON) {
        v_theta = (PI * !s_x); 
        player->v_y = 0;
    }
    else if (a_v_y < EPSILON && a_v_x < EPSILON) {
        player->v_x = player->v_y = 0;
        return;
    }
    else {
        v_theta = atan2(player->v_y, player->v_x);
    }

    double x_drag = max(DRAG * pow(cos(v_theta), 2), MIN_DRAG); 
    double y_drag = max(DRAG * pow(sin(v_theta), 2), MIN_DRAG);

    if (player->v_x < 0)
        player->v_x += x_drag;
    else
        player->v_x -= x_drag;

    if (player->v_y < 0)
        player->v_y += y_drag;
    else
        player->v_y -= y_drag;

    player->v_x = player->v_x * (s_x == player->v_x > 0);
    player->v_y = player->v_y * (s_y == player->v_y > 0);

    double velocity = pow(pow(player->v_x, 2) + pow(player->v_y, 2), 0.5);
    if (velocity > VELMAX || velocity < -VELMAX) {
        player->v_y = sin(v_theta) * VELMAX;
        player->v_x = cos(v_theta) * VELMAX;
    }
}

void physics_tick(player_info_t* player) {
    apply_drag(player);

    player->x += player->v_x;
    if (player->x > TILE_SIZE) {
        player->tile_x += 1;
        player->x -= TILE_SIZE;
    }
    else if (player->x < 0) {
        player->tile_x -= 1;
        player->x += TILE_SIZE;
    }

    player->y += player->v_y;
    if (player->y > TILE_SIZE) {
        player->tile_y += 1;
        player->y -= TILE_SIZE;
    }
    else if (player->y < 0) {
        player->tile_y -= 1;
        player->y += TILE_SIZE;
    }
}
