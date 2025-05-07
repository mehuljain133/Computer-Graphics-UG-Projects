/* Geometric Modeling: Polygon Mesh Representation, Cubic Polynomial curves (Hermite and
Bezier) */

#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

// ==========================
// ===== POLYGON MESH ======
// ==========================
struct Vertex {
    float x, y, z;
};

vector<Vertex> vertices = {
    {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
    {0.5,  0.5, -0.5}, {-0.5,  0.5, -0.5},
    {-0.5, -0.5,  0.5}, {0.5, -0.5,  0.5},
    {0.5,  0.5,  0.5}, {-0.5,  0.5,  0.5}
};

// Each face is a quad (indices to vertices)
int faces[6][4] = {
    {0, 1, 2, 3}, {4, 5, 6, 7},
    {0, 1, 5, 4}, {2, 3, 7, 6},
    {0, 3, 7, 4}, {1, 2, 6, 5}
};

void drawPolygonMesh() {
    glColor3f(0.5, 0.8, 0.8);
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 4; j++) {
            Vertex v = vertices[faces[i][j]];
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
}

// ==========================
// ==== HERMITE CURVE =======
// ==========================

Vertex hermite(float t, Vertex p0, Vertex p1, Vertex r0, Vertex r1) {
    float h1 = 2*t*t*t - 3*t*t + 1;
    float h2 = -2*t*t*t + 3*t*t;
    float h3 = t*t*t - 2*t*t + t;
    float h4 = t*t*t - t*t;
    return {
        h1*p0.x + h2*p1.x + h3*r0.x + h4*r1.x,
        h1*p0.y + h2*p1.y + h3*r0.y + h4*r1.y,
        0.0
    };
}

void drawHermite() {
    Vertex p0 = {-0.8, -0.2, 0}, p1 = {0.8, 0.2, 0};
    Vertex r0 = {1, 1, 0}, r1 = {-1, 1, 0};

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.0; t += 0.01) {
        Vertex p = hermite(t, p0, p1, r0, r1);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

// ==========================
// ==== BEZIER CURVE =========
// ==========================

Vertex bezier(float t, Vertex p0, Vertex p1, Vertex p2, Vertex p3) {
    float u = 1 - t;
    float b0 = u * u * u;
    float b1 = 3 * u * u * t;
    float b2 = 3 * u * t * t;
    float b3 = t * t * t;

    return {
        b0*p0.x + b1*p1.x + b2*p2.x + b3*p3.x,
        b0*p0.y + b1*p1.y + b2*p2.y + b3*p3.y,
        0.0
    };
}

void drawBezier() {
    Vertex p0 = {-0.9, -0.6, 0}, p1 = {-0.3, 0.8, 0};
    Vertex p2 = {0.3, -0.8, 0}, p3 = {0.9, 0.6, 0};

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.0; t += 0.01) {
        Vertex p = bezier(t, p0, p1, p2, p3);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

// ==========================
// ===== OPENGL INIT =========
// ==========================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 2.5, 0, 0, 0, 0, 1, 0);

    drawPolygonMesh();
    drawHermite();
    drawBezier();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// ==========================
// ====== MAIN FUNCTION =====
// ==========================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Geometric Modeling: Mesh, Hermite, Bezier - PhD Code");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
