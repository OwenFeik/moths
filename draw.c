#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "util.h"

#define WIDTH 1920
#define HEIGHT 1080

#define TRAIL_CLOUD_RAD 10
#define TRAIL_CLOUD_LIFE 20

typedef struct {
    float x;
    float y;
    int age;
} timed_point_t;

list_t trail;

void vertex(float x, float y) {
    glVertex2f(x / WIDTH, y / HEIGHT);
}

void calc_view_port(void) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    float ratio = min((float) w / WIDTH, (float) h / HEIGHT);

    int vp_width = (int) (ratio * WIDTH);
    int vp_height = (int) (ratio * HEIGHT);

    glViewport(
        abs((w - vp_width) / 2), 
        abs((h - vp_height) / 2), 
        vp_width, 
        vp_height
    );
}

void draw_circle(float x, float y, float r, int n) {
    glBegin(GL_POLYGON);
    
    for (int i = 0; i < n; i++) {
        float theta = TAU * i / n;
        vertex(x + r * cos(theta), y + r * sin(theta));
    }

    glEnd();
}

void art_init() {
    trail = new_list();
}

void draw_player(float x, float y, float dir) {
    float rot = 3 * PI / 4;

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);
        vertex(x + 40 * cos(dir), y + 40 * sin(dir));
        vertex(x + 30 * cos(dir - rot), y + 30 * sin(dir - rot));
        vertex(x + 10 * cos(dir + PI), y + 10 * sin(dir + PI));
        vertex(x + 30 * cos(dir + rot), y + 30 * sin(dir + rot));
    glEnd();
}

void draw_trail_point(void* ptr) {
    timed_point_t* point = (timed_point_t*) ptr;
    
    point->age += 1;
    draw_circle(point->x, point->y, TRAIL_CLOUD_RAD / sqrt(point->age), 10);
}

void art_tick(float p_x, float p_y, float dir) {
    glClear(GL_COLOR_BUFFER_BIT);

    draw_player(p_x, p_y, dir);

    timed_point_t* new_cloud = malloc(sizeof(timed_point_t));
    new_cloud->x = p_x;
    new_cloud->y = p_y;
    new_cloud->age = 0;
    
    push_to_list(&trail, new_cloud);
    for_each(&trail, draw_trail_point);

    if (((timed_point_t*) trail.base->obj)->age > TRAIL_CLOUD_LIFE) {
        timed_point_t* old_cloud = pop_from_list(&trail);
        free(old_cloud);
    }

    glFlush();
}
