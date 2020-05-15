#include <ctime>
#include <string>
#include <iostream>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Point {
    float x, y;
    Point() { x = 0.0; y = 0.0; }

    Point(float _x, float _y): x(_x), y(_y) {}
};

struct View {
    float vert, horiz, transx, transy, zoom;
    View() { vert = 0.0; horiz = 0.0; transx = 0.0; transy = 0.0; zoom = 0.0; }
};

Point* lastMousePt = new Point;
Point* delta = new Point;
View* view = new View;

void mouse(int button, int state, int x, int y) {
    if(button == 0 || button == 2) {
        lastMousePt->x = x;
        lastMousePt->y = y;
    }
}

void motion(int x, int y) {
    // Point* delta = new Point(x - lastMousePt->x, y - lastMousePt->y);
    delta->x = x - lastMousePt->x;
    delta->y = y - lastMousePt->y;
    lastMousePt->x = x;
    lastMousePt->y = y;
    std::cout << delta->x << ":" << delta->y << std::endl;
}

void display() {
    // update view
    view->vert += delta->y * 0.4;
    view->horiz += delta->x * 0.4;

    // perform transformation
    

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
    glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Game of Life");

    glutMotionFunc(motion);
    glutMouseFunc(mouse);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
