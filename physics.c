#include <math.h>
#include <stdio.h>

#include "util.h"

#define MAP_SIZE 255

#define RIGHT -1
#define LEFT 1
#define FORWARD 1
#define BACKWARD -1

#define DTHETA 0.1
#define DVEL 1

#define DRAG 0.5
#define VELMAX 5

float theta = 0;
float v_x = 0, v_y = 0;
float p_x = 0, p_y = 0;

float to_range(float val, float geq, float leq) {
    if (val >= leq)
        return leq;
    if (val <= geq)
        return geq;
    return val;
}

void apply_rotation(float dir) {
    theta += DTHETA * dir;
    if (theta > TAU)
        theta -= TAU;
    else if (theta < -TAU)
        theta += TAU;
}

void apply_acceleration(float dir) {
    v_x += DVEL * dir * cos(theta);
    v_x = to_range(v_x, -VELMAX, VELMAX);
    v_y += DVEL * dir * sin(theta);
    v_y = to_range(v_y, -VELMAX, VELMAX);
}

void apply_drag() {

    int s_x = v_x > 0;
    int s_y = v_y > 0;

    if (v_x > 0)
        v_x -= DRAG;
    else if (v_x < 0)
        v_x += DRAG;
    
    if (v_y > 0)
        v_y -= DRAG;
    else if (v_y < 0)
        v_y += DRAG;

    v_x = v_x * (s_x == v_x > 0);
    v_y = v_y * (s_y == v_y > 0);
}

void physics_tick() {
    p_x += v_x;
    p_x = to_range(p_x, 0, MAP_SIZE);

    p_y += v_y;
    p_y = to_range(p_y, 0, MAP_SIZE);

    apply_drag();
}
