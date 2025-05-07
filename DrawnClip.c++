/* Drawing and clipping primitives: Raster scan line, circle and ellipse drawing algorithms,
Polygon filling, line clipping and polygon clipping algorithms */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    int x, y;
};

// ==== Drawing Algorithms ====

void drawPixel(int x, int y) {
    glBegin(GL_POINTS); glVertex2i(x, y); glEnd();
}

void bresenhamLine(Point p1, Point p2) {
    int x = p1.x, y = p1.y;
    int dx = abs(p2.x - p1.x), dy = abs(p2.y - p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    bool isSteep = dy > dx;

    if (isSteep) swap(dx, dy);

    int err = 2 * dy - dx;

    for (int i = 0; i <= dx; i++) {
        drawPixel(x, y);
        if (err > 0) {
            if (isSteep) x += sx;
            else y += sy;
            err -= 2 * dx;
        }
        if (isSteep) y += sy;
        else x += sx;
        err += 2 * dy;
    }
}

void midpointCircle(Point center, int radius) {
    int x = 0, y = radius;
    int d = 1 - radius;
    while (x <= y) {
        drawPixel(center.x + x, center.y + y);
        drawPixel(center.x - x, center.y + y);
        drawPixel(center.x + x, center.y - y);
        drawPixel(center.x - x, center.y - y);
        drawPixel(center.x + y, center.y + x);
        drawPixel(center.x - y, center.y + x);
        drawPixel(center.x + y, center.y - x);
        drawPixel(center.x - y, center.y - x);
        if (d < 0) d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void midpointEllipse(Point center, int rx, int ry) {
    float dx, dy, d1, d2, x = 0, y = ry;

    d1 = (ry * ry) - (rx * rx * ry) + (0.25f * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    while (dx < dy) {
        drawPixel(center.x + x, center.y + y);
        drawPixel(center.x - x, center.y + y);
        drawPixel(center.x + x, center.y - y);
        drawPixel(center.x - x, center.y - y);
        if (d1 < 0) {
            x++;
            dx = dx + (2 * ry * ry);
            d1 += dx + (ry * ry);
        } else {
            x++; y--;
            dx += (2 * ry * ry);
            dy -= (2 * rx * rx);
            d1 += dx - dy + (ry * ry);
        }
    }

    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
         (rx * rx * ry * ry);

    while (y >= 0) {
        drawPixel(center.x + x, center.y + y);
        drawPixel(center.x - x, center.y + y);
        drawPixel(center.x + x, center.y - y);
        drawPixel(center.x - x, center.y - y);
        if (d2 > 0) {
            y--;
            dy -= (2 * rx * rx);
            d2 += (rx * rx) - dy;
        } else {
            y--; x++;
            dx += (2 * ry * ry);
            dy -= (2 * rx * rx);
            d2 += dx - dy + (rx * rx);
        }
    }
}

// ==== Polygon Fill (Scanline Fill) ====

vector<Point> polygonPoints;

void scanlineFill() {
    int n = polygonPoints.size();
    if (n < 3) return;

    int ymin = polygonPoints[0].y, ymax = polygonPoints[0].y;
    for (auto& p : polygonPoints) {
        ymin = min(ymin, p.y);
        ymax = max(ymax, p.y);
    }

    for (int y = ymin; y <= ymax; y++) {
        vector<int> intersections;
        for (int i = 0; i < n; i++) {
            Point p1 = polygonPoints[i];
            Point p2 = polygonPoints[(i + 1) % n];
            if (p1.y == p2.y) continue;
            if ((y >= min(p1.y, p2.y)) && (y < max(p1.y, p2.y))) {
                int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                intersections.push_back(x);
            }
        }
        sort(intersections.begin(), intersections.end());
        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            for (int x = intersections[i]; x <= intersections[i + 1]; x++)
                drawPixel(x, y);
        }
    }
}

// ==== Line Clipping (Cohen–Sutherland) ====

const int INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
int xmin = 100, xmax = 400, ymin = 100, ymax = 300;

int computeOutCode(int x, int y) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

void cohenSutherlandClip(Point p1, Point p2) {
    int out1 = computeOutCode(p1.x, p1.y);
    int out2 = computeOutCode(p2.x, p2.y);
    bool accept = false;

    while (true) {
        if (!(out1 | out2)) { accept = true; break; }
        else if (out1 & out2) break;
        else {
            int outcode = out1 ? out1 : out2;
            int x, y;
            if (outcode & TOP) {
                x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
                y = ymax;
            } else if (outcode & BOTTOM) {
                x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
                y = ymin;
            } else if (outcode & RIGHT) {
                y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
                x = xmax;
            } else {
                y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
                x = xmin;
            }
            if (outcode == out1) { p1.x = x; p1.y = y; out1 = computeOutCode(p1.x, p1.y); }
            else { p2.x = x; p2.y = y; out2 = computeOutCode(p2.x, p2.y); }
        }
    }
    if (accept) bresenhamLine(p1, p2);
}

// ==== Polygon Clipping (Sutherland–Hodgman) ====

vector<Point> sutherlandHodgmanClip(const vector<Point>& poly) {
    vector<Point> output = poly;
    vector<Point> input;
    int bounds[4] = {xmin, xmax, ymin, ymax};

    for (int edge = 0; edge < 4; ++edge) {
        input = output;
        output.clear();
        for (size_t i = 0; i < input.size(); ++i) {
            Point A = input[i];
            Point B = input[(i + 1) % input.size()];
            auto inside = [&](Point p) {
                if (edge == 0) return p.x >= xmin;
                if (edge == 1) return p.x <= xmax;
                if (edge == 2) return p.y >= ymin;
                return p.y <= ymax;
            };
            auto intersect = [&]() {
                float m = (float)(B.y - A.y) / (B.x - A.x + 1e-5);
                Point p;
                if (edge == 0) { p.x = xmin; p.y = A.y + m * (xmin - A.x); }
                else if (edge == 1) { p.x = xmax; p.y = A.y + m * (xmax - A.x); }
                else if (edge == 2) { p.y = ymin; p.x = A.x + (ymin - A.y) / m; }
                else { p.y = ymax; p.x = A.x + (ymax - A.y) / m; }
                return p;
            };
            if (inside(A) && inside(B)) output.push_back(B);
            else if (inside(A) && !inside(B)) output.push_back(intersect());
            else if (!inside(A) && inside(B)) {
                output.push_back(intersect());
                output.push_back(B);
            }
        }
    }
    return output;
}

// ==== OpenGL Setup ====

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing primitives
    glColor3f(1, 0, 0);
    bresenhamLine({50, 50}, {200, 150});
    midpointCircle({300, 300}, 50);
    midpointEllipse({500, 200}, 80, 40);

    // Polygon and scanline fill
    glColor3f(0, 1, 0);
    polygonPoints = {{200, 100}, {250, 200}, {300, 150}, {275, 80}};
    scanlineFill();

    // Line Clipping
    glColor3f(1, 1, 0);
    cohenSutherlandClip({50, 200}, {450, 250});

    // Polygon Clipping
    glColor3f(0, 1, 1);
    vector<Point> clippedPoly = sutherlandHodgmanClip(polygonPoints);
    glBegin(GL_LINE_LOOP);
    for (auto& p : clippedPoly) glVertex2i(p.x, p.y);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("All-in-One: Drawing and Clipping Primitives");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
