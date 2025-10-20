// Bar.cpp
#include "Bar.h"

using namespace std;

Bar::Bar(Rectangle rect, vector<float> vals) : Graph(rect), values(vals) {}

void Bar::draw() {
  DrawRectangleLinesEx(area, 2, GRAY);
  if (values.empty()) return;
  float barW = area.width / values.size();
  float maxV = *std::max_element(values.begin(), values.end());
  for (size_t i = 0; i < values.size(); ++i) {
    float h = (values[i] / maxV) * area.height;
    DrawRectangle(area.x + i * barW, area.y + area.height - h, barW - 2, h,
                  GREEN);
  }
}

void Bar::simulation(float dt) {
  for (auto& v : values) {
    v += (rand() % 3 - 1) * 2.f;
  }
  draw();
}
