#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "map.h"
#include "util.h"

#define VP_WIDTH 1920
#define VP_HEIGHT 1080

#define TRAIL_CLOUD_RAD 10
#define TRAIL_CLOUD_LIFE 20

list_t trail;

void vertex(float x, float y) {
    glVertex2f(x / VP_WIDTH, y / VP_HEIGHT);
}

void calc_view_port(void) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    float ratio = min((float) w / VP_WIDTH, (float) h / VP_HEIGHT);

    int vp_width = (int) (ratio * VP_WIDTH);
    int vp_height = (int) (ratio * VP_HEIGHT);

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
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    trail = new_list();
}

void draw_player(player_info_t* player) {
    float rot = 3 * PI / 4;
    float dir = player->dir;

    float x = VP_WIDTH / 2;
    float y = VP_HEIGHT / 2;

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);
        vertex(x + 40 * cos(dir), y + 40 * sin(dir));
        vertex(x + 30 * cos(dir - rot), y + 30 * sin(dir - rot));
        vertex(x + 10 * cos(dir + PI), y + 10 * sin(dir + PI));
        vertex(x + 30 * cos(dir + rot), y + 30 * sin(dir + rot));
    glEnd();
}

void draw_trail_point(void* ptr) {
    val_point_t* point = (val_point_t*) ptr;
    
    point->val += 1;
    draw_circle(point->x, point->y, TRAIL_CLOUD_RAD / sqrt(point->val), 10);
}

void draw_tile(float p_x, float p_y, tile_t* tile) {
    list_node_t* node = tile->objs.base;
    while (node) {
        val_point_t* point = (val_point_t*) node->obj;
        float dx = point->x - p_x;
        float dy = point->y - p_y; 
        
        int on_screen_x = fabs(dx) - point->val < VP_WIDTH / 2;
        int on_screen_y = fabs(dy) - point->val < VP_HEIGHT / 2; 
        
        if (on_screen_x && on_screen_y) {
            draw_circle(
                VP_WIDTH / 2 + dx,
                VP_HEIGHT / 2 + dy,
                point->val,
                min(point->val / 4, 10)
            );
        }
    }
}

void draw_map(player_info_t* player) {
    float p_x = player->x;
    float p_y = player->y;
    int p_tile_x = player->tile_x;
    int p_tile_y = player->tile_y;

    draw_tile(p_x, p_y, get_player_tile());

    int off_left = p_x - VP_WIDTH / 2 < 0;
    int off_right = p_x + VP_WIDTH / 2 > TILE_SIZE;
    int off_bottom = p_y - VP_HEIGHT / 2 < 0; 
    int off_top = p_y + VP_HEIGHT / 2 > TILE_SIZE;

    if (off_left)
        draw_tile(p_x - TILE_SIZE, p_y, get_tile(p_tile_x - 1, p_tile_y));
    if (off_left && off_top)
        draw_tile(
            p_x - TILE_SIZE,
            p_y + TILE_SIZE,
            get_tile(p_tile_x - 1, p_tile_y + 1)
        );
    if (off_top)
        draw_tile(p_x, p_y + TILE_SIZE, get_tile(p_tile_x, p_tile_y + 1));
    if (off_top && off_right)
        draw_tile(
            p_x + TILE_SIZE,
            p_y + TILE_SIZE,
            get_tile(p_tile_x + 1, p_tile_y + 1)
        );
    if (off_right)
        draw_tile(p_x + TILE_SIZE, p_y, get_tile(p_tile_x + 1, p_tile_y));
    if (off_right && off_bottom)
        draw_tile(
            p_x + TILE_SIZE,
            p_y - TILE_SIZE,
            get_tile(p_tile_x + 1, p_tile_y - 1)
        );
    if (off_bottom)
        draw_tile(p_x, p_y - TILE_SIZE, get_tile(p_tile_x, p_tile_y - 1));
    if (off_left && off_bottom)
        draw_tile(
            p_x - TILE_SIZE,
            p_y - TILE_SIZE,
            get_tile(p_tile_x - 1, p_tile_y - 1)
        );
}

void art_tick(player_info_t* player) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(.25, .25, 25);
    glBegin(GL_POLYGON);
        vertex(0, 0);
        vertex(0, VP_HEIGHT);
        vertex(VP_WIDTH, VP_HEIGHT);
        vertex(VP_WIDTH, 0);
    glEnd();
    glColor3f(1, 1, 1);

    draw_player(player);

    val_point_t* new_cloud = malloc(sizeof(val_point_t));
    new_cloud->x = player->x;
    new_cloud->y = player->y;
    new_cloud->val = 0;
    
    push_to_list(&trail, new_cloud);
    for_each(&trail, draw_trail_point);

    if (((val_point_t*) trail.base->obj)->val > TRAIL_CLOUD_LIFE) {
        val_point_t* old_cloud = pop_from_list(&trail);
        free(old_cloud);
    }

    glFlush();
}
