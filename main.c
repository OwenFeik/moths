/*
 * hello.c
 * This is a simple, introductory OpenGL program.
 */

#include <freetype2/ft2build.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "util.h"
#include "physics.c"
#include "draw.c"

int keys[32] = {0};

void calc_view_port(void) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    int size = min(w, h);

    glViewport(abs((w - size) / 2), abs((h - size) / 2), size, size);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.25, 0.25, 0.25);

    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glFlush();
}

void display(void) {
    calc_view_port();
}

void draw_box(float x, float y, float dir) {
    x /= 255;
    y /= 255;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(x + 0.02 * cos(dir), y + 0.02 * sin(dir), 0);
        glVertex3f(
            x + 0.015 * cos(dir - (3 * PI / 4)), 
            y + 0.015 * sin(dir - (3 * PI / 4)), 
            0
        );
        glVertex3f(x + 0.005 * cos(dir + PI), y + 0.005 * sin(dir + PI), 0);
        glVertex3f(
            x + 0.015 * cos(dir + (3 * PI / 4)), 
            y + 0.015 * sin(dir + (3 * PI / 4)), 
            0
        );
    glEnd();

    draw_circle(x + 0.03 * cos(dir + PI), y + 0.03 * sin(dir + PI), 0.005, 10);

    glFlush();
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
    // if (get_key_down('a'))
    // if (get_key_down('d'))

}

void do_tick(int tick) {
    handle_key_downs();
    physics_tick();
    draw_box(p_x, p_y, theta);
    glutTimerFunc(16, do_tick, tick + 1);
}

int main(int argc, char** argv) {
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
