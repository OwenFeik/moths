#include <math.h>
#include <stdio.h>

#include "util.h"

#define MAP_WIDTH 1920
#define MAP_HEIGHT 1080

#define RIGHT -1
#define LEFT 1
#define FORWARD 1
#define BACKWARD -1

#define DTHETA 0.1
#define DVEL 3

#define DRAG 1
#define MIN_DRAG 0.1
#define VELMAX 10

double theta = 0;
double v_x = 0, v_y = 0;
double p_x = 0, p_y = 0;

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
    v_y += DVEL * dir * sin(theta);
}

void apply_drag() {
    int s_x = v_x > 0;
    int s_y = v_y > 0;

    double v_theta;
    double a_v_x = fabs(v_x);
    double a_v_y = fabs(v_y);

    if (a_v_x < EPSILON && a_v_y > EPSILON) {
        v_theta = (PI / 2) * (s_y * 1 + !s_y * -1);
        v_x = 0;
    }
    else if (a_v_y < EPSILON && a_v_x > EPSILON) {
        v_theta = (PI * !s_x); 
        v_y = 0;
    }
    else if (a_v_y < EPSILON && a_v_x < EPSILON) {
        v_x = v_y = 0;
        return;
    }
    else {
        v_theta = atan2(v_y, v_x);
    }

    double x_drag = max(DRAG * pow(cos(v_theta), 2), MIN_DRAG); 
    double y_drag = max(DRAG * pow(sin(v_theta), 2), MIN_DRAG);

    if (v_x < 0)
        v_x += x_drag;
    else
        v_x -= x_drag;

    if (v_y < 0)
        v_y += y_drag;
    else
        v_y -= y_drag;

    v_x = v_x * (s_x == v_x > 0);
    v_y = v_y * (s_y == v_y > 0);
}

void physics_tick() {
    v_x = to_range(v_x, -VELMAX, VELMAX);
    v_y = to_range(v_y, -VELMAX, VELMAX);

    apply_drag();

    p_x += v_x;
    p_x = to_range(p_x, 0, MAP_WIDTH);

    p_y += v_y;
    p_y = to_range(p_y, 0, MAP_HEIGHT);
}
