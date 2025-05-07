// Write a program to implement Bresenham’s line drawing algorithm

#include <GL/glut.h>
#include <iostream>
using namespace std;

int x1, y1, x2, y2;

void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    bool swap = false;

    if (dy > dx) {
        std::swap(dx, dy);
        swap = true;
    }

    int p = 2 * dy - dx;
    int x = x1, y = y1;

    for (int i = 0; i <= dx; i++) {
        plot(x, y);

        if (p >= 0) {
            if (swap)
                x += sx;
            else
                y += sy;
            p -= 2 * dx;
        }

        if (swap)
            y += sy;
        else
            x += sx;

        p += 2 * dy;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);  // Red line
    bresenhamLine(x1, y1, x2, y2);
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);  // White background
    glColor3f(0, 0, 0);        // Black drawing color
    gluOrtho2D(0, 500, 0, 500); // 2D coordinate system
}

int main(int argc, char** argv) {
    cout << "Enter starting point (x1 y1): ";
    cin >> x1 >> y1;
    cout << "Enter ending point (x2 y2): ";
    cin >> x2 >> y2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
