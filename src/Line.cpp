#include "Line.h"

using namespace std;

Line::Line(Rectangle rect, vector<Vector2> pts) : Graph(rect), points(pts) {}

void Line::draw() {
    DrawRectangleLines(area.x, area.y, area.width, area.height, GRAY);
    for (size_t i = 1; i < points.size(); i++) {
        DrawLineV(points[i - 1], points[i], RED);
    }
}
