#include "Line.h"

#include <algorithm>
#include <string>

#include "raylib.h"

using namespace std;

#define LINE_COLOR CLITERAL(Color){80, 180, 255, 255}
#define AXIS_COLOR CLITERAL(Color){180, 180, 200, 255}

// Helper function
static float mapY(float price, float minP, float maxP, float y0, float height) {
  if (maxP == minP) return y0 + height / 2;
  float norm = (price - minP) / (maxP - minP);
  return y0 + height - norm * height;
}

// Constructor
Line::Line(Rectangle rect, vector<string> t, vector<float> p)
    : Graph(rect), dates(t), prices(p) {
  if (!prices.empty()) {
    yMin = *min_element(prices.begin(), prices.end());
    yMax = *max_element(prices.begin(), prices.end());
    float margin = (yMax - yMin) * 0.05f;
    yMin -= margin;
    yMax += margin;
  }
  currentIndex = clamp((int)(prices.size() * 0.25f), 0, (int)prices.size() - 1);
}

// format date (YYYY-MM-DD → DD/MM/YY)
static string formatDate(string dateStr) {
  if (dateStr.size() < 10) return dateStr;
  return dateStr.substr(8, 2) + "/" + dateStr.substr(5, 2) + "/" +
         dateStr.substr(2, 2);
}

void Line::draw() {
  if (prices.empty()) return;

  // Draw chart border
  DrawRectangleLinesEx(area, 2, AXIS_COLOR);

  float stepX = area.width / (float)(prices.size() - 1);

  // Draw the y-axis grid lines and labels
  for (int i = 0; i <= yTicks; i++) {
    float yVal = yMin + (yMax - yMin) * (i / (float)yTicks);
    float yPos = mapY(yVal, yMin, yMax, area.y, area.height);
    DrawLine(area.x, yPos, area.x + area.width, yPos, Fade(AXIS_COLOR, 0.3f));

    DrawText(TextFormat("%.2f", yVal), area.x - 70, yPos - 10, 16, TEXT_COLOR);
  }

  // Draw the labels for the x-axis
  int totalPoints = prices.size();
  for (int i = 0; i <= xTicks; i++) {
    int idx = (int)((i / (float)xTicks) * (totalPoints - 1));
    float xPos = area.x + idx * stepX;
    if (idx >= 0 && idx < dates.size()) {
      DrawLine(xPos, area.y + area.height, xPos, area.y + area.height + 5,
               AXIS_COLOR);
      string shortDate = formatDate(dates[idx]);
      DrawText(shortDate.c_str(), xPos - 30, area.y + area.height + 10, 16,
               TEXT_COLOR);
    }
  }

  // Animated price line
  Vector2 prev = {area.x, mapY(prices[0], yMin, yMax, area.y, area.height)};
  for (int i = 1; i <= currentIndex && i < prices.size(); i++) {
    float x = area.x + i * stepX;
    float y = mapY(prices[i], yMin, yMax, area.y, area.height);
    DrawLineV(prev, {x, y}, LINE_COLOR);
    prev = {x, y};
  }

  // ==== Draw current marker ====
  if (currentIndex < prices.size()) {
    float y = mapY(prices[currentIndex], yMin, yMax, area.y, area.height);
    float x = area.x + currentIndex * stepX;
    DrawCircle(x, y, 5, RAYWHITE);

    // Date + price label near marker
    DrawText(TextFormat("%s", formatDate(dates[currentIndex]).c_str()), x + 10,
             y - 25, 16, TEXT_COLOR);
    DrawText(TextFormat("$%.2f", prices[currentIndex]), x + 10, y - 5, 16,
             TEXT_COLOR);
  }
}

void Line::simulation(float dt) {
  if (!animating || prices.empty()) return;
  timeAccumulator += dt;
    //Holds time
  if (timeAccumulator >= 0.05f) {
    currentIndex = min(currentIndex + 1, (int)prices.size() - 1);
    timeAccumulator = 0.0f;
  }

  draw();
}

void Line::startAnimation() { animating = true; }
void Line::stopAnimation() { animating = false; }

void Line::startAnimationFrom(int index) {
  currentIndex = clamp(index, 0, (int)prices.size() - 1);
  animating = true;
}

string Line::getPausedDate() const {
  if (dates.empty()) return "N/A";
  return dates[min(currentIndex, (int)dates.size() - 1)];
}

string Line::getCurrentDate() const {
    if (dates.empty()) return "N/A";
    int idx = std::clamp(currentIndex, 0, (int)dates.size() - 1);
    return dates[idx];
}

int Line::getCurrentIndex() const {
    return currentIndex;
}

float Line::getCurrentPrice() const {
    if (currentIndex >= 0 && currentIndex < (int)prices.size())
        return prices[currentIndex];
    return 0.0f;
}


