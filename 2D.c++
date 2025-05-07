// Write a program to apply various 2D transformations on a 2D object

#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

// Define the initial object (a triangle)
vector<pair<float, float>> object = {
    {100.0f, 100.0f}, {200.0f, 100.0f}, {150.0f, 200.0f}
};

// Transformation matrices
float tx = 0.0f, ty = 0.0f;  // Translation parameters
float sx = 1.0f, sy = 1.0f;  // Scaling factors
float angle = 0.0f;          // Rotation angle
float shx = 0.0f, shy = 0.0f; // Shearing factors

// Function to apply translation
void translateObject() {
    for (auto &point : object) {
        point.first += tx;
        point.second += ty;
    }
}

// Function to apply scaling
void scaleObject() {
    for (auto &point : object) {
        point.first *= sx;
        point.second *= sy;
    }
}

// Function to apply rotation
void rotateObject() {
    float rad = angle * 3.14159265358979f / 180.0f; // Convert angle to radians
    for (auto &point : object) {
        float xNew = point.first * cos(rad) - point.second * sin(rad);
        float yNew = point.first * sin(rad) + point.second * cos(rad);
        point.first = xNew;
        point.second = yNew;
    }
}

// Function to apply shearing
void shearObject() {
    for (auto &point : object) {
        point.first += shx * point.second;
        point.second += shy * point.first;
    }
}

// Function to draw the object
void drawObject() {
    glBegin(GL_POLYGON); // Drawing the object (a triangle)
    for (auto &point : object) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
}

// Function to apply all transformations
void applyTransformations() {
    // Apply translation
    translateObject();
    // Apply scaling
    scaleObject();
    // Apply rotation
    rotateObject();
    // Apply shearing
    shearObject();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Apply transformations to the object
    applyTransformations();

    // Set object color (Green for filled object)
    glColor3f(0.0f, 1.0f, 0.0f);
    drawObject();

    glFlush();
}

// Keyboard function to handle user input for transformations
void keyboard(unsigned char key, int x, int y) {
    if (key == 't') {
        tx += 10.0f;  // Apply translation on x-axis
        ty += 10.0f;  // Apply translation on y-axis
    } else if (key == 's') {
        sx *= 1.1f;  // Scale the object by 10% on x-axis
        sy *= 1.1f;  // Scale the object by 10% on y-axis
    } else if (key == 'r') {
        angle += 10.0f;  // Rotate the object by 10 degrees
    } else if (key == 'h') {
        shx += 0.1f;  // Apply shearing along the x-axis
        shy += 0.1f;  // Apply shearing along the y-axis
    }

    glutPostRedisplay(); // Redraw the window with updated transformations
}

// Initialize OpenGL settings
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background to white
    gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f); // 2D orthographic projection
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Transformations");

    init();

    glutDisplayFunc(display); // Display callback
    glutKeyboardFunc(keyboard); // Keyboard callback

    glutMainLoop();
    return 0;
}
