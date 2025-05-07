// Write a program to apply various 3D transformations on a 3D object and then apply parallel and perspective projection on it

#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Transformation parameters
float tx = 0.0f, ty = 0.0f, tz = 0.0f;  // Translation
float sx = 1.0f, sy = 1.0f, sz = 1.0f;  // Scaling
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;  // Rotation angles

// Cube vertices
GLfloat cubeVertices[8][3] = {
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f}
};

// Cube edges (connecting vertices)
GLint cubeEdges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, 
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

// Function to apply translation to the cube
void translateCube() {
    for (int i = 0; i < 8; i++) {
        cubeVertices[i][0] += tx;
        cubeVertices[i][1] += ty;
        cubeVertices[i][2] += tz;
    }
}

// Function to apply scaling to the cube
void scaleCube() {
    for (int i = 0; i < 8; i++) {
        cubeVertices[i][0] *= sx;
        cubeVertices[i][1] *= sy;
        cubeVertices[i][2] *= sz;
    }
}

// Function to apply rotation to the cube
void rotateCube() {
    // Rotation along X-axis
    float tempY, tempZ;
    for (int i = 0; i < 8; i++) {
        tempY = cubeVertices[i][1];
        tempZ = cubeVertices[i][2];
        cubeVertices[i][1] = tempY * cos(angleX) - tempZ * sin(angleX);
        cubeVertices[i][2] = tempY * sin(angleX) + tempZ * cos(angleX);
    }

    // Rotation along Y-axis
    float tempX, tempZ;
    for (int i = 0; i < 8; i++) {
        tempX = cubeVertices[i][0];
        tempZ = cubeVertices[i][2];
        cubeVertices[i][0] = tempX * cos(angleY) + tempZ * sin(angleY);
        cubeVertices[i][2] = -tempX * sin(angleY) + tempZ * cos(angleY);
    }

    // Rotation along Z-axis
    for (int i = 0; i < 8; i++) {
        tempX = cubeVertices[i][0];
        tempY = cubeVertices[i][1];
        cubeVertices[i][0] = tempX * cos(angleZ) - tempY * sin(angleZ);
        cubeVertices[i][1] = tempX * sin(angleZ) + tempY * cos(angleZ);
    }
}

// Function to draw the cube
void drawCube() {
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {
        int startVertex = cubeEdges[i][0];
        int endVertex = cubeEdges[i][1];
        glVertex3f(cubeVertices[startVertex][0], cubeVertices[startVertex][1], cubeVertices[startVertex][2]);
        glVertex3f(cubeVertices[endVertex][0], cubeVertices[endVertex][1], cubeVertices[endVertex][2]);
    }
    glEnd();
}

// Function to apply parallel projection
void parallelProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5, 5, -5, 5); // Orthogonal projection (parallel projection)
}

// Function to apply perspective projection
void perspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 20.0f); // Perspective projection
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply transformations
    translateCube();
    scaleCube();
    rotateCube();

    // Set color for the cube
    glColor3f(1.0f, 0.0f, 0.0f); // Red color

    // Draw the cube
    drawCube();

    glFlush();
    glutSwapBuffers();
}

// Function to set the perspective and apply transformations
void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background to white
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D effects
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply projection (parallel or perspective based on key press)
    perspectiveProjection(); // Switch to parallelProjection() to see parallel projection
}

void keyboard(unsigned char key, int x, int y) {
    // Apply transformations based on key press
    if (key == 't') {
        tx += 0.1f; // Move the object along the X-axis
    } else if (key == 's') {
        sy += 0.1f; // Scale along Y-axis
    } else if (key == 'r') {
        angleZ += 10.0f; // Rotate along the Z-axis by 10 degrees
    }

    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Transformations and Projections");

    setup();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
