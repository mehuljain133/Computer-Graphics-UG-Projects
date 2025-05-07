// Write a program to clip a polygon using Sutherland Hodgeman algorithm

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Clipping window boundaries
const int xmin = 100, ymin = 100, xmax = 400, ymax = 400;

// Define the polygon vertices (x, y)
vector<pair<int, int>> polygon = {
    {50, 50}, {450, 50}, {450, 450}, {50, 450}
};

// Function to plot a polygon
void plotPolygon(const vector<pair<int, int>>& poly) {
    glBegin(GL_LINE_LOOP);
    for (auto& vertex : poly) {
        glVertex2i(vertex.first, vertex.second);
    }
    glEnd();
}

// Function to compute the cross product of two vectors (p1 -> p2) x (p2 -> p3)
int crossProduct(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3) {
    return (p2.first - p1.first) * (p3.second - p1.second) -
           (p2.second - p1.second) * (p3.first - p1.first);
}

// Function to clip the polygon against one edge of the clipping window
vector<pair<int, int>> clipEdge(vector<pair<int, int>> subjectPolygon, pair<int, int> clipStart, pair<int, int> clipEnd) {
    vector<pair<int, int>> outputPolygon;
    
    // Loop through each edge of the polygon
    int n = subjectPolygon.size();
    for (int i = 0; i < n; i++) {
        pair<int, int> currentPoint = subjectPolygon[i];
        pair<int, int> previousPoint = subjectPolygon[(i + n - 1) % n];

        // Calculate the cross product to check if the point is inside or outside the clipping edge
        int crossPrev = crossProduct(clipStart, clipEnd, previousPoint);
        int crossCurr = crossProduct(clipStart, clipEnd, currentPoint);

        // Case 1: If the current point is inside the clipping window (cross product >= 0)
        if (crossCurr >= 0) {
            // If the previous point is outside, compute the intersection point
            if (crossPrev < 0) {
                float t = (float)crossProduct(previousPoint, currentPoint, clipStart) / (crossProduct(previousPoint, currentPoint, clipEnd) - crossProduct(previousPoint, currentPoint, clipStart));
                int x = previousPoint.first + t * (currentPoint.first - previousPoint.first);
                int y = previousPoint.second + t * (currentPoint.second - previousPoint.second);
                outputPolygon.push_back({x, y});
            }
            outputPolygon.push_back(currentPoint);
        } else if (crossPrev >= 0) {
            // Case 2: If the previous point is inside, compute the intersection point
            float t = (float)crossProduct(previousPoint, currentPoint, clipStart) / (crossProduct(previousPoint, currentPoint, clipEnd) - crossProduct(previousPoint, currentPoint, clipStart));
            int x = previousPoint.first + t * (currentPoint.first - previousPoint.first);
            int y = previousPoint.second + t * (currentPoint.second - previousPoint.second);
            outputPolygon.push_back({x, y});
        }
    }

    return outputPolygon;
}

// Function to clip the polygon against all four edges of the clipping window
vector<pair<int, int>> sutherlandHodgmanClip(vector<pair<int, int>> polygon) {
    // Clip against left edge
    polygon = clipEdge(polygon, {xmin, ymin}, {xmin, ymax});
    // Clip against right edge
    polygon = clipEdge(polygon, {xmax, ymin}, {xmax, ymax});
    // Clip against bottom edge
    polygon = clipEdge(polygon, {xmin, ymin}, {xmax, ymin});
    // Clip against top edge
    polygon = clipEdge(polygon, {xmin, ymax}, {xmax, ymax});
    
    return polygon;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the clipping window
    glColor3f(0, 0, 1); // Blue for clipping window
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmin, ymax);
    glVertex2i(xmax, ymax);
    glVertex2i(xmax, ymin);
    glEnd();

    // Draw the original polygon (red)
    glColor3f(1, 0, 0); // Red for original polygon
    plotPolygon(polygon);

    // Clip the polygon using the Sutherland-Hodgman algorithm
    vector<pair<int, int>> clippedPolygon = sutherlandHodgmanClip(polygon);

    // Draw the clipped polygon (green)
    glColor3f(0, 1, 0); // Green for clipped polygon
    plotPolygon(clippedPolygon);

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
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
