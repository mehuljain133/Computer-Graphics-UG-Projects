// Write a program to fill a polygon using Scan line fill algorithm

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Define the polygon vertices (x, y)
vector<pair<int, int>> polygon = {
    {100, 100}, {200, 200}, {300, 100}, {250, 50}, {150, 50}
};

// Function to plot the polygon
void plotPolygon(const vector<pair<int, int>>& poly) {
    glBegin(GL_LINE_LOOP);
    for (auto& vertex : poly) {
        glVertex2i(vertex.first, vertex.second);
    }
    glEnd();
}

// Function to sort edges by y-coordinate
bool compareEdge(pair<int, int> p1, pair<int, int> p2) {
    return p1.second < p2.second;
}

// Function to fill the polygon using the Scan Line Fill Algorithm
void scanLineFill(vector<pair<int, int>> poly) {
    int ymax = 0;
    int ymin = 9999;

    // Finding the ymin and ymax coordinates
    for (auto& p : poly) {
        if (p.second < ymin) ymin = p.second;
        if (p.second > ymax) ymax = p.second;
    }

    // Loop through each scanline (from ymin to ymax)
    for (int y = ymin; y <= ymax; y++) {
        vector<int> intersections;

        // Find intersections of the current scanline with the polygon edges
        for (int i = 0; i < poly.size(); i++) {
            int x1 = poly[i].first;
            int y1 = poly[i].second;
            int x2 = poly[(i + 1) % poly.size()].first;
            int y2 = poly[(i + 1) % poly.size()].second;

            // Check if the scanline intersects with the edge
            if ((y1 > y && y2 <= y) || (y1 <= y && y2 > y)) {
                // Find the intersection point of the edge with the scanline
                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                intersections.push_back(x);
            }
        }

        // Sort intersections
        sort(intersections.begin(), intersections.end());

        // Fill the pixels between the pairs of intersections
        for (int i = 0; i < intersections.size(); i += 2) {
            glBegin(GL_LINES);
            glVertex2i(intersections[i], y);
            glVertex2i(intersections[i + 1], y);
            glEnd();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the original polygon (blue)
    glColor3f(0, 0, 1); // Blue for the polygon outline
    plotPolygon(polygon);

    // Fill the polygon using Scan Line Fill Algorithm (green)
    glColor3f(0, 1, 0); // Green for filling
    scanLineFill(polygon);

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 500, 0, 500); // 2D coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scan Line Polygon Fill Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
