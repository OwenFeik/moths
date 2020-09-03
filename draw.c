#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "map.h"
#include "util.h"

#define DRAW_TILE_BOUNDARIES 1

#define VP_WIDTH 1920
#define VP_HEIGHT 1080

#define TRAIL_CLOUD_RAD 5
#define TRAIL_CLOUD_LIFE 20

list_t trail;

typedef struct {
    float x;
    float y;
    int tile_x;
    int tile_y;
    int age;
} trail_point_t;

void vertex(float x, float y) {
    glVertex2f(x, y);
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
    glOrtho(0, VP_WIDTH, 0, VP_HEIGHT, -1, 1);

    trail = new_list();
}

void draw_player(player_info_t* player) {
    float rot = 3 * PI / 4;
    float dir = player->dir;

    float x = VP_WIDTH / 2;
    float y = VP_HEIGHT / 2;

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);
        vertex(x + 20 * cos(dir), y + 20 * sin(dir));
        vertex(x + 15 * cos(dir - rot), y + 15 * sin(dir - rot));
        vertex(x + 5 * cos(dir + PI), y + 5 * sin(dir + PI));
        vertex(x + 15 * cos(dir + rot), y + 15 * sin(dir + rot));
    glEnd();
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

        node = node->next;
    }
}

void draw_map(player_info_t* player) {
    float p_x = player->x;
    float p_y = player->y;
    int p_tile_x = player->tile_x;
    int p_tile_y = player->tile_y;

    tile_t* player_tile = get_player_tile(player);
    draw_tile(p_x, p_y, player_tile);

    int off_left = p_x - VP_WIDTH / 2 < 0;
    int off_right = p_x + VP_WIDTH / 2 > TILE_SIZE;
    int off_bottom = p_y - VP_HEIGHT / 2 < 0; 
    int off_top = p_y + VP_HEIGHT / 2 > TILE_SIZE;

    if (off_left)
        draw_tile(p_x + TILE_SIZE, p_y, get_tile(p_tile_x - 1, p_tile_y));
    if (off_left && off_top)
        draw_tile(
            p_x + TILE_SIZE,
            p_y - TILE_SIZE,
            get_tile(p_tile_x - 1, p_tile_y + 1)
        );
    if (off_top)
        draw_tile(p_x, p_y - TILE_SIZE, get_tile(p_tile_x, p_tile_y + 1));
    if (off_top && off_right)
        draw_tile(
            p_x - TILE_SIZE,
            p_y - TILE_SIZE,
            get_tile(p_tile_x + 1, p_tile_y + 1)
        );
    if (off_right)
        draw_tile(p_x - TILE_SIZE, p_y, get_tile(p_tile_x + 1, p_tile_y));
    if (off_right && off_bottom)
        draw_tile(
            p_x - TILE_SIZE,
            p_y + TILE_SIZE,
            get_tile(p_tile_x + 1, p_tile_y - 1)
        );
    if (off_bottom)
        draw_tile(p_x, p_y + TILE_SIZE, get_tile(p_tile_x, p_tile_y - 1));
    if (off_left && off_bottom)
        draw_tile(
            p_x + TILE_SIZE,
            p_y + TILE_SIZE,
            get_tile(p_tile_x - 1, p_tile_y - 1)
        );
}

void draw_trail(player_info_t* player) {
    list_node_t* node = trail.base;
    while (node) {
        trail_point_t* point = (trail_point_t*) node->obj;
        point->age += 1;
        draw_circle(
            (point->x + (TILE_SIZE * (point->tile_x - player->tile_x)) - player->x) + VP_WIDTH / 2,
            (point->y + (TILE_SIZE * (point->tile_y - player->tile_y)) - player->y) + VP_HEIGHT / 2,
            TRAIL_CLOUD_RAD / sqrt(point->age),
            10
        );

        node = node->next;
    }


}

void trail_tick(player_info_t* player) {
    draw_trail(player);

    trail_point_t* new_cloud = malloc(sizeof(trail_point_t));
    new_cloud->x = player->x;
    new_cloud->y = player->y;
    new_cloud->tile_x = player->tile_x;
    new_cloud->tile_y = player->tile_y;
    new_cloud->age = 0;
    push_to_list(&trail, new_cloud);

    if (((trail_point_t*) trail.base->obj)->age > TRAIL_CLOUD_LIFE) {
        trail_point_t* old_cloud = pop_from_list(&trail);
        free(old_cloud);
    }
}

void art_tick(player_info_t* player) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    trail_tick(player);
    draw_player(player);
    draw_map(player);

    glFlush();
}
