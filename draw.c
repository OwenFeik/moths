#include <GL/glut.h>

#include "util.h"

void draw_circle(float x, float y, float r, int n) {
    glBegin(GL_POLYGON);
    
    for (int i = 0; i < n; i++) {
        float theta = TAU * i / n;

        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }

    glEnd();
}
