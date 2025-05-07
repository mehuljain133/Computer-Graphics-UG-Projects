/* Visible Surface determination and Surface Rendering: Z-buffer algorithm, List-priority
algorithm and area subdivision algorithm for visible surface determination. Illumination and
shading models, RGB color model and Basics of Computer Animation. */

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

// ==============================
// Globals
// ==============================
float angle = 0.0;  // Animation rotation angle
bool useZBuffer = true;
bool usePhongShading = true;

// ==============================
// Light and Material Properties
// ==============================
GLfloat light_pos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// ==============================
// Painter's Algorithm Simulation
// ==============================
struct ColoredQuad {
    float zOrder;
    GLfloat color[3];
    GLfloat vertices[4][3];
};

vector<ColoredQuad> quads;

void createQuads() {
    quads.clear();
    quads.push_back({ -0.5f, {1, 0, 0}, {{-1, -1, -0.5}, {1, -1, -0.5}, {1, 1, -0.5}, {-1, 1, -0.5}} });
    quads.push_back({ -0.2f, {0, 1, 0}, {{-0.5, -0.5, -0.2}, {0.5, -0.5, -0.2}, {0.5, 0.5, -0.2}, {-0.5, 0.5, -0.2}} });
    quads.push_back({ -1.0f, {0, 0, 1}, {{-0.3, -0.3, -1.0}, {0.3, -0.3, -1.0}, {0.3, 0.3, -1.0}, {-0.3, 0.3, -1.0}} });
}

bool compareZ(const ColoredQuad& a, const ColoredQuad& b) {
    return a.zOrder < b.zOrder;
}

void drawPainterQuads() {
    sort(quads.begin(), quads.end(), compareZ);
    for (const auto& quad : quads) {
        glColor3fv(quad.color);
        glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++)
            glVertex3fv(quad.vertices[i]);
        glEnd();
    }
}

// ==============================
// 3D Object with Illumination
// ==============================
void drawLitCube() {
    glPushMatrix();
    glRotatef(angle, 1.0, 1.0, 0.0);
    glutSolidTeapot(0.5);
    glPopMatrix();
}

// ==============================
// Display Function
// ==============================
void display() {
    if (useZBuffer) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(1.5, 1.5, 2.5, 0, 0, 0, 0, 1, 0);

    if (useZBuffer) {
        // Z-buffer with lighting
        glEnable(GL_LIGHTING);
        drawLitCube();
    } else {
        // Painter’s algorithm simulation
        glDisable(GL_LIGHTING);
        drawPainterQuads();
    }

    glutSwapBuffers();
}

// ==============================
// Animation Timer
// ==============================
void timer(int v) {
    angle += 1.0;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// ==============================
// Keyboard
// ==============================
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'z': useZBuffer = !useZBuffer; break;
        case 'p': usePhongShading = !usePhongShading; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

// ==============================
// Initialization
// ==============================
void init() {
    createQuads();
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    if (usePhongShading) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
    }

    glClearColor(0.1, 0.1, 0.1, 1);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// ==============================
// Main Function
// ==============================
int main(int argc, char** argv) {
    cout << "Controls:\n"
         << "[Z] Toggle Z-buffer / Painter's algorithm\n"
         << "[P] Toggle Phong shading\n"
         << "[ESC] Exit\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Visible Surface, Shading, RGB, Animation - PhD Code");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}
