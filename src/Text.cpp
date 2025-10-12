#include "Text.h"

Text::Text(Rectangle rect, string ph, int maxLen)
    : InputElement(rect), placeholder(ph), content(""), active(false), maxLength(maxLen) {}

void Text::update() {
    // Click inside box = activate input
    if (CheckCollisionPointRec(GetMousePosition(), area)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        active = false;
    }

    if (active) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && content.length() < maxLength)
                content += (char)key;
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !content.empty())
            content.pop_back();
    }
}

void Text::draw() {
    DrawRectangleRec(area, RAYWHITE);
    DrawRectangleLinesEx(area, 2, active ? BLUE : GRAY);

    string display = content.empty() ? placeholder : content;
    Color displayColor = content.empty() ? LIGHTGRAY : BLACK;

    DrawText(display.c_str(), area.x + 8, area.y + 10, 18, displayColor);

    // Blinking cursor
    if (active && (GetTime() * 2 - (int)(GetTime() * 2)) < 0.5) DrawText("|", area.x + 8 + MeasureText(display.c_str(), 18), area.y + 10, 18, BLACK);
}

bool Text::isActive() {
    return active;
}

string Text::getContent(){
    return content;
}

void Text::setContent(string text){
    content = text;
}

void Text::clear(){
    content.clear();
}
