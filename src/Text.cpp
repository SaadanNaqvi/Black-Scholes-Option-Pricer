#include "Text.h"
using namespace std;

Text::Text(Rectangle rect, string text) : InputElement(rect), placeholder(text) {}

void Text::draw() {
    DrawRectangleRec(area, RAYWHITE);
    DrawRectangleLinesEx(area, 2, BLUE);
    DrawText(placeholder.c_str(), area.x + 8, area.y + 10, 18, BLUE);
}
