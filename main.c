// #include <freetype2/ft2build.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "util.h"
#include "physics.c"
#include "draw.c"
#include "map.h"

#define PRINT_TICK_RATE 0

char keys[32] = {0};
player_info_t* player;
int start;

void display(void) {
    calc_view_port();
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
    if (get_key_down('q'))
        apply_rotation(player, LEFT);
    if (get_key_down('e'))
        apply_rotation(player, RIGHT);
    if (get_key_down('w'))
        apply_acceleration(player, FORWARD);
    if (get_key_down('s'))
        apply_acceleration(player, BACKWARD);
}

void do_tick(int tick) {
    handle_key_downs();
    physics_tick(player);
    art_tick(player);
    glutTimerFunc(1000 / TICK_RATE, do_tick, tick + 1);

    if (PRINT_TICK_RATE) {
        if (tick > 0 && tick % 60 == 0) {
            printf("%d\n", tick / (time(NULL) - start)); 
        }
    }
}

int main(int argc, char** argv) {
    player = (player_info_t*) malloc(sizeof(player_info_t));
    map_init();

    start = time(NULL); 
    srand(start);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);

    // glutInitWindowSize(250, 500); 
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("main");

    art_init();

    glEnable(GL_MULTISAMPLE_ARB);

    glutKeyboardFunc(key_down); 
    glutKeyboardUpFunc(key_up);
    
    glutTimerFunc(0, do_tick, 0);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
