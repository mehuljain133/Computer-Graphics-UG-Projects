// Write a program to clip a line using Cohen and Sutherland line clipping algorithm

#include <GL/glut.h>
#include <iostream>
using namespace std;

// Define the clipping window boundaries
const int xmin = 100, ymin = 100, xmax = 400, ymax = 400;

// Outcode constants for Cohen-Sutherland algorithm
#define INSIDE 0  // 0000
#define LEFT 1    // 0001
#define RIGHT 2   // 0010
#define BOTTOM 4  // 0100
#define TOP 8     // 1000

// Define the clipping window's boundaries
void clipWindow() {
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmin, ymax);
    glVertex2i(xmax, ymax);
    glVertex2i(xmax, ymin);
    glEnd();
}

// Compute the outcode for a point (x, y) using the clipping window
int computeOutcode(int x, int y) {
    int code = INSIDE;

    if (x < xmin)         // to the left of the window
        code |= LEFT;
    else if (x > xmax)    // to the right of the window
        code |= RIGHT;
    if (y < ymin)         // below the window
        code |= BOTTOM;
    else if (y > ymax)    // above the window
        code |= TOP;

    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
void cohenSutherlandClip(int &x1, int &y1, int &x2, int &y2) {
    int outcode1 = computeOutcode(x1, y1);
    int outcode2 = computeOutcode(x2, y2);
    bool accept = false;

    while (true) {
        if ((outcode1 == 0) && (outcode2 == 0)) { // Both endpoints are inside
            accept = true;
            break;
        } else if (outcode1 & outcode2) { // Both endpoints are outside the clipping window
            break;
        } else { // Some portion of the line is inside
            int x, y;
            int outcodeOut = outcode1 ? outcode1 : outcode2;

            // Find intersection point
            if (outcodeOut & TOP) { // Line intersects the top edge
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (outcodeOut & BOTTOM) { // Line intersects the bottom edge
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (outcodeOut & RIGHT) { // Line intersects the right edge
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else if (outcodeOut & LEFT) { // Line intersects the left edge
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            // Update the point outside the clipping region
            if (outcodeOut == outcode1) {
                x1 = x;
                y1 = y;
                outcode1 = computeOutcode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                outcode2 = computeOutcode(x2, y2);
            }
        }
    }

    if (accept) {
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw clipping window
    glColor3f(0, 0, 1); // Blue color for clipping window
    clipWindow();

    // Line to be clipped (x1, y1, x2, y2)
    int x1 = 50, y1 = 50, x2 = 350, y2 = 350;
    glColor3f(1, 0, 0); // Red color for the line
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();

    // Apply Cohen-Sutherland clipping
    cohenSutherlandClip(x1, y1, x2, y2);

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);  // White background
    glColor3f(0, 0, 0);        // Black for line and window boundary
    gluOrtho2D(0, 500, 0, 500); // 2D coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
