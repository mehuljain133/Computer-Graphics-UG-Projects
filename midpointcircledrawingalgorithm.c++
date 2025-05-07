//  Write a program to implement mid-point circle drawing algorithm

#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

// Circle center and radius
int xc, yc, r;

void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void midPointCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int p = 1 - r;  // Mid-point decision parameter

    plot(x, y); // Plot the initial points

    while (x < y) {
        x++;

        if (p < 0) {
            p += 2*x + 1;
        } else {
            y--;
            p += 2*(x - y) + 1;
        }

        plot(x, y);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Mid-point Circle Drawing
    midPointCircle(xc, yc, r);

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);  // White background
    glColor3f(0, 0, 0);        // Black color for drawing
    gluOrtho2D(0, 500, 0, 500); // 2D coordinate system
}

int main(int argc, char** argv) {
    cout << "Enter the center of the circle (xc yc): ";
    cin >> xc >> yc;
    cout << "Enter the radius of the circle: ";
    cin >> r;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mid-Point Circle Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
