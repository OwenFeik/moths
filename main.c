// #include <freetype2/ft2build.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "util.h"
#include "physics.c"
#include "draw.c"
#include "map.c"

int keys[32] = {0};

void display(void) {
    calc_view_port();
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
} 

void key_down(unsigned char key, int x, int y) {
    keys[key / 8] |= (1 << (key % 8));
    // if (key == ' ') {
    //     update_vel(0, 30);
    // }
}

void key_up(unsigned char key, int x, int y) {
    keys[key / 8] ^= (1 << (key % 8));
}

int get_key_down(unsigned char key) {
    return keys[key / 8] & (1 << (key % 8));
}

void handle_key_downs() {
    if (get_key_down('w'))
        apply_acceleration(FORWARD);
    if (get_key_down('s'))
        apply_acceleration(BACKWARD);
    if (get_key_down('q'))
        apply_rotation(LEFT);
    if (get_key_down('e'))
        apply_rotation(RIGHT);
}

void do_tick(int tick) {
    handle_key_downs();
    physics_tick();
    art_tick(p_x, p_y, theta);
    glutTimerFunc(1000 / TICK_RATE, do_tick, tick + 1);
}

int main(int argc, char** argv) {
    art_init();

    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);

    // glutInitWindowSize(250, 500); 
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("main");
    glEnable(GL_MULTISAMPLE_ARB);

    glutKeyboardFunc(key_down); 
    glutKeyboardUpFunc(key_up);
    
    init();
    
    glutTimerFunc(0, do_tick, 0);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
