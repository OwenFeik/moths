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

#define DRAG -0.5
#define VELMAX 10

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

    printf("vy: %.2f, vx: %.2f\n", v_y, v_x);

    double v_theta;
    if (v_x > EPSILON && v_y > EPSILON)
        v_theta = atan(abs(v_y) / abs(v_x));
    if (v_y > EPSILON)
        v_theta = PI / 2;
    else {
        v_x = v_y = 0;
        return;
    }

    printf("successfully assigned vt\n");

    float x_drag = DRAG * pow(cos(v_theta), 2); 
    float y_drag = DRAG * pow(sin(v_theta), 2);

    if (x_drag == x_drag)
        v_x += x_drag;
    if (y_drag == y_drag)
        v_y += y_drag;

    printf("vt: %.2f, xd: %.2f, yd: %.2f, sum: %.2f\n", v_theta, x_drag, y_drag, x_drag + y_drag);


    v_x = v_x * (s_x == v_x > 0);
    v_y = v_y * (s_y == v_y > 0);
}

void physics_tick() {
    apply_drag();

    p_x += v_x;
    p_x = to_range(p_x, 0, MAP_WIDTH);

    p_y += v_y;
    p_y = to_range(p_y, 0, MAP_HEIGHT);
}
