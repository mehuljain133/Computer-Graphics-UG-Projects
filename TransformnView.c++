/* Transformation and Viewing: 2D and 3D Geometric Transformations, 2D and 3D Viewing
Transformations (Projections- Parallel and Perspective), Vanishing points. */

#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

// 3D point structure
struct Point3D {
    float x, y, z;
};

// Cube vertices
Point3D cube[] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},   // back face
    {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}     // front face
};

// Transformation parameters
float angleX = 0, angleY = 0, angleZ = 0;
float scale = 1.0;
float tx = 0, ty = 0, tz = 0;

bool usePerspective = true;

// ==== Apply 3D Transformations ====

void applyTransform(Point3D& p) {
    // Scale
    p.x *= scale; p.y *= scale; p.z *= scale;

    // Rotate X
    float y1 = p.y * cos(angleX) - p.z * sin(angleX);
    float z1 = p.y * sin(angleX) + p.z * cos(angleX);
    p.y = y1; p.z = z1;

    // Rotate Y
    float x2 = p.x * cos(angleY) + p.z * sin(angleY);
    float z2 = -p.x * sin(angleY) + p.z * cos(angleY);
    p.x = x2; p.z = z2;

    // Rotate Z
    float x3 = p.x * cos(angleZ) - p.y * sin(angleZ);
    float y3 = p.x * sin(angleZ) + p.y * cos(angleZ);
    p.x = x3; p.y = y3;

    // Translate
    p.x += tx; p.y += ty; p.z += tz;
}

// ==== Draw Wireframe Cube ====

void drawCube() {
    glBegin(GL_LINE_LOOP); // back face
    for (int i = 0; i < 4; ++i) {
        Point3D p = cube[i];
        applyTransform(p);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glBegin(GL_LINE_LOOP); // front face
    for (int i = 4; i < 8; ++i) {
        Point3D p = cube[i];
        applyTransform(p);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    // Connect corresponding vertices
    for (int i = 0; i < 4; ++i) {
        glBegin(GL_LINES);
        Point3D p1 = cube[i], p2 = cube[i + 4];
        applyTransform(p1); applyTransform(p2);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glEnd();
    }
}

// ==== Display Function ====

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (usePerspective)
        gluLookAt(4, 4, 4, 0, 0, 0, 0, 1, 0); // Perspective
    else
        glTranslatef(-1.5f, 0.0f, -6.0f); // Parallel projection simulation

    glColor3f(1.0, 1.0, 1.0);
    drawCube();

    glutSwapBuffers();
}

// ==== Keyboard Input ====

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w': angleX += 0.1; break;
        case 's': angleX -= 0.1; break;
        case 'a': angleY += 0.1; break;
        case 'd': angleY -= 0.1; break;
        case 'q': angleZ += 0.1; break;
        case 'e': angleZ -= 0.1; break;
        case '+': scale *= 1.1; break;
        case '-': scale /= 1.1; break;
        case 'p': usePerspective = !usePerspective; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

// ==== Setup OpenGL ====

void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1.0, 100.0); // Perspective projection
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 1);
}

int main(int argc, char** argv) {
    cout << "Controls:\n"
         << "Rotate: [W/S] X, [A/D] Y, [Q/E] Z\n"
         << "Scale: [+ / -]\n"
         << "Toggle Projection: [P]\n"
         << "Exit: [ESC]\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D/3D Transformations and Viewing - PhD Level");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
