/*
 * hello.c
 * This is a simple, introductory OpenGL program.
 */

#include <freetype2/ft2build.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

int p_x = 128, p_y = 128;
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

void draw_box(float left, float top) {
    float bot = (1.0 - (top / 255));
    float lft = left / 255;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(lft, bot, 0.0);
        glVertex3f(lft + 0.05, bot, 0.0);
        glVertex3f(lft + 0.05, bot + 0.05, 0.0);
        glVertex3f(lft, bot + 0.05, 0.0);
    glEnd();

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
}

void key_up(unsigned char key, int x, int y) {
    keys[key / 8] ^= (1 << (key % 8));
}

int get_key_down(unsigned char key) {
    return keys[key / 8] & (1 << (key % 8));
}

void handle_key_downs() {
    if (get_key_down('w') && p_y > 0)
        p_y -= 1;
    if (get_key_down('a') && p_x > 0)
        p_x -= 1;
    if (get_key_down('s') && p_y < 255)
        p_y += 1;
    if (get_key_down('d') && p_x < 255)
        p_x += 1;
}

void do_tick(int tick) {
    handle_key_downs();
    draw_box(p_x, p_y);
    glutTimerFunc(16, do_tick, tick + 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // glutInitWindowSize(250, 500); 
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("main");

    glutKeyboardFunc(key_down); 
    glutKeyboardUpFunc(key_up);
    
    init();
    
    glutTimerFunc(0, do_tick, 0);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
