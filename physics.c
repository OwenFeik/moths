#include <stdlib.h>

#define G 10
#define VMAX_X 10
#define VMIN_X -10
#define VMAX_Y 50
#define VMIN_Y -10
#define PMIN_X 0
#define PMAX_X 255
#define PMIN_Y 20
#define PMAX_Y 255

int p_x = 128, p_y = 128;
int p_v_x = 0, p_v_y = 0;

int to_range(int val, int geq, int leq) {
    if (val >= leq)
        return leq;
    if (val <= geq)
        return geq;
    return val;
}

void update_vel(int a_x, int a_y) {
    p_v_x = to_range(p_v_x + a_x, VMIN_X, VMAX_X);
    p_v_y = to_range(p_v_y + a_y, VMIN_Y, VMAX_Y);
}

void physics_tick(void) {
    p_v_y -= 1;

    p_x = to_range(p_x + p_v_x, PMIN_X, PMAX_X);
    p_y = to_range(p_y + p_v_y, PMIN_Y, PMAX_Y);

    if (p_v_x > 0)
        p_v_x -= 1;
    else if (p_v_x < 0)
        p_v_x += 1;

    if (p_v_y > 0)
        p_v_y -= 1;
}
