// Write a program to draw Hermite /Bezier curve

#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

// Control points for the Hermite and Bezier curves
struct Point {
    float x, y;
};

Point p0 = {50.0f, 100.0f}; // Control point 1
Point p1 = {150.0f, 200.0f}; // Control point 2
Point p2 = {250.0f, 200.0f}; // Control point 3
Point p3 = {350.0f, 100.0f}; // Control point 4

Point tangent1 = {100.0f, 0.0f};  // Tangent vector at p0
Point tangent2 = {-100.0f, 0.0f}; // Tangent vector at p3

// Function to calculate Hermite curve
void hermiteCurve(Point p0, Point p1, Point tangent1, Point tangent2, int segments) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float h0 = 2 * t * t * t - 3 * t * t + 1;
        float h1 = -2 * t * t * t + 3 * t * t;
        float h2 = t * t * t - 2 * t * t + t;
        float h3 = t * t * t - t * t;

        float x = h0 * p0.x + h1 * p1.x + h2 * tangent1.x + h3 * tangent2.x;
        float y = h0 * p0.y + h1 * p1.y + h2 * tangent1.y + h3 * tangent2.y;

        glVertex2f(x, y);
    }
    glEnd();
}

// Function to calculate Bezier curve
void bezierCurve(Point p0, Point p1, Point p2, Point p3, int segments) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float x = (1 - t) * (1 - t) * (1 - t) * p0.x + 
                  3 * t * (1 - t) * (1 - t) * p1.x + 
                  3 * t * t * (1 - t) * p2.x + 
                  t * t * t * p3.x;

        float y = (1 - t) * (1 - t) * (1 - t) * p0.y + 
                  3 * t * (1 - t) * (1 - t) * p1.y + 
                  3 * t * t * (1 - t) * p2.y + 
                  t * t * t * p3.y;

        glVertex2f(x, y);
    }
    glEnd();
}

// Function to display the curves
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set color for Hermite curve (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    hermiteCurve(p0, p3, tangent1, tangent2, 100);  // Draw Hermite curve

    // Set color for Bezier curve (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    bezierCurve(p0, p1, p2, p3, 100);  // Draw Bezier curve

    glFlush();
    glutSwapBuffers();
}

// Function to set up OpenGL environment
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set background color to white
    gluOrtho2D(0.0f, 400.0f, 0.0f, 400.0f); // Define 2D orthogonal projection
}

// Main function to initialize and start the program
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Hermite and Bezier Curves");

    init();

    glutDisplayFunc(display);  // Register the display function

    glutMainLoop();  // Start the main loop
    return 0;
}
