#include "Line.h"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;

#define AXIS_COLOR   CLITERAL(Color){180, 180, 200, 255}
#define GRID_COLOR   CLITERAL(Color){55, 65, 100, 255}
#define TEXT_COLOR   CLITERAL(Color){235, 235, 245, 255}
#define LINE_COLOR   CLITERAL(Color){80, 180, 255, 255}

Line::Line(Rectangle rect, vector<string> t,
           vector<float> p)
    : Graph(rect), prices(p), dates(t)
{
    if (!prices.empty()) {
        yMin = *min_element(prices.begin(), prices.end());
        yMax = *max_element(prices.begin(), prices.end());
        float margin = (yMax - yMin) * 0.05f;
        yMin -= margin;
        yMax += margin;
    }
}

// Helper for mapping a price to Y coordinate
static float mapY(float price, float minP, float maxP, float y0, float height) {
    if (maxP == minP) return y0 + height / 2;
    float norm = (price - minP) / (maxP - minP);
    return y0 + height - norm * height;
}

void Line::draw() {
    if (prices.empty()) {
        DrawText("No stock data", area.x + 20, area.y + area.height / 2, 20, RED);
        return;
    }

    DrawRectangleLinesEx(area, 2, AXIS_COLOR);

    int n = prices.size();
    float stepX = area.width / (float)(n - 1);

    // ======= GRID + AXES =======
    for (int i = 0; i <= xTicks; i++) {
        float x = area.x + i * (area.width / xTicks);
        DrawLine(x, area.y, x, area.y + area.height, GRID_COLOR);
    }
    for (int i = 0; i <= yTicks; i++) {
        float y = area.y + i * (area.height / yTicks);
        DrawLine(area.x, y, area.x + area.width, y, GRID_COLOR);
    }

    DrawLine(area.x, area.y, area.x, area.y + area.height, AXIS_COLOR); // Y axis
    DrawLine(area.x, area.y + area.height, area.x + area.width,
             area.y + area.height, AXIS_COLOR); // X axis

    // ======= Y-AXIS LABELS (Actual Prices) =======
    for (int i = 0; i <= yTicks; i++) {
        float price = yMax - (yMax - yMin) * (i / (float)yTicks);
        float y = area.y + (area.height / yTicks) * i - 6;
        DrawText(TextFormat("%.2f", price), area.x - 65, y, 16, TEXT_COLOR);
    }

    // ======= X-AXIS LABELS (Actual Dates) =======
    if (!dates.empty()) {
        int skip = max(1, (int)dates.size() / xTicks);
        for (int i = 0; i < dates.size(); i += skip) {
            float x = area.x + i * stepX - 15;
            string label = dates[i];
            // Shorten the label to keep it readable (YYYY-MM-DD → MM-DD)
            if (label.size() >= 10) label = label.substr(5, 5);
            DrawText(label.c_str(), x, area.y + area.height + 8, 14, TEXT_COLOR);
        }
    }

    // Axis titles
    DrawText("Price ($)", area.x - 70, area.y - 30, 18, TEXT_COLOR);
    DrawText("Date →", area.x + area.width / 2 - 20, area.y + area.height + 28, 18, TEXT_COLOR);

    // ======= PRICE LINE =======
    Vector2 prev = {area.x, mapY(prices[0], yMin, yMax, area.y, area.height)};
    for (size_t i = 1; i < prices.size(); i++) {
        float x = area.x + i * stepX;
        float y = mapY(prices[i], yMin, yMax, area.y, area.height);
        DrawLineV(prev, {x, y}, LINE_COLOR);
        prev = {x, y};
    }
}

void Line::simulation(float dt) {
    if (prices.empty()) return;

    simulationProgress += dt * 0.4f;
    int visible = min((int)(simulationProgress * prices.size()), (int)prices.size());
    if (visible < 2) return;

    float stepX = area.width / (float)(prices.size() - 1);
    Vector2 prev = {area.x, mapY(prices[0], yMin, yMax, area.y, area.height)};

    // draw partial line while animating
    for (int i = 1; i < visible; i++) {
        float x = area.x + i * stepX;
        float y = mapY(prices[i], yMin, yMax, area.y, area.height);
        DrawLineV(prev, {x, y}, LINE_COLOR);
        prev = {x, y};
    }

    // Draw static axes and labels over animation
    draw();
}
