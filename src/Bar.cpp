#include "Bar.h"

Bar::Bar(Rectangle rect, vector<float> vals): Graph(rect), values(vals) {}

void Bar::draw() {
    float barWidth = area.width / (float)values.size();
    for (int i = 0; i < values.size(); i++) {
        Color c = (values[i] < 0) ? RED : ((i == 1) ? GREEN : PURPLE);
        float barHeight = (values[i] / 800.0f) * area.height;
        DrawRectangle(area.x + i * (barWidth + 10), area.y + area.height - barHeight, barWidth, barHeight, c);
        DrawText(TextFormat("%.0f", values[i]), area.x + i * (barWidth + 10) + 10, area.y + area.height - barHeight - 20, 18, BLACK);
    }
}

void Bar::simulation(float dt) {
    // Simple animation: fluctuate values slightly
    for (auto &v : values) {
        v += (rand() % 3 - 1) * 2.0f; // small random delta
    }
    draw();
}

