/* Introduction: Introduction to Graphics systems, Basic elements of Computer graphics,
Applications of computer graphics. Architecture of Raster and Random scan display devices,
input/output devices. */

#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

enum Mode { RASTER, VECTOR };
Mode currentMode = RASTER;

vector<pair<float, float>> points;

void drawRasterLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1); glVertex2f(x2, y2);
    glEnd();
}

void drawVectorLine(float x1, float y1, float x2, float y2) {
    // Simulate beam movement for random scan display
    glColor3f(0.0, 1.0, 0.0); // Green beam
    glBegin(GL_POINTS);
    for (float t = 0; t <= 1.0; t += 0.001) {
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);
        glVertex2f(x, y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Coordinate system lines
    glColor3f(0.5, 0.5, 0.5);
    drawRasterLine(-1, 0, 1, 0); // X-axis
    drawRasterLine(0, -1, 0, 1); // Y-axis

    // Draw based on mode
    glColor3f(1.0, 0.0, 0.0);
    for (size_t i = 0; i < points.size(); i += 2) {
        if (i + 1 < points.size()) {
            if (currentMode == RASTER)
                drawRasterLine(points[i].first, points[i].second, points[i+1].first, points[i+1].second);
            else
                drawVectorLine(points[i].first, points[i].second, points[i+1].first, points[i+1].second);
        }
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        float fx = (2.0 * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0;
        float fy = 1.0 - (2.0 * y) / glutGet(GLUT_WINDOW_HEIGHT);
        points.push_back({fx, fy});
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': points.clear(); break;
        case 'm': currentMode = (currentMode == RASTER) ? VECTOR : RASTER; break;
        case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    cout << "Computer Graphics System Demo\n";
    cout << "[Click] to add points (every 2 points form a line)\n";
    cout << "[m] Toggle Raster/Vector Mode\n[c] Clear screen\n[ESC] Exit\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Computer Graphics - Raster vs Vector");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
