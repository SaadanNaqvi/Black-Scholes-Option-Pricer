#include "Button.h"
using namespace std;

Button::Button(Rectangle rect, string lbl) : InputElement(rect), label(lbl) {}

void Button::draw() {
    DrawRectangleRec(area, LIGHTGRAY);
    DrawRectangleLinesEx(area, 2, DARKGRAY);
    DrawText(label.c_str(), area.x + 15, area.y + 10, 20, RED);
}

bool Button::isClicked() {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), area);
}
