#include "Dropdown.h"
using namespace std;

Dropdown::Dropdown(Rectangle rect, vector<string> opts, int defaultIdx)
    : area(rect), options(opts), selected(defaultIdx), open(false) {}

void Dropdown::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, area)) {
            open = !open;
        } else if (open) {
            for (int i = 0; i < options.size(); i++) {
                Rectangle opt = {area.x, area.y + (i + 1) * area.height, area.width, area.height};
                if (CheckCollisionPointRec(m, opt)) {
                    selected = i;
                    open = false;
                }
            }
        } else {
            open = false;
        }
    }
}

void Dropdown::draw() {
    DrawRectangleRec(area, RAYWHITE);
    DrawRectangleLinesEx(area, 2, GRAY);
    DrawText(options[selected].c_str(), area.x + 8, area.y + 8, 18, BLACK);

    if (open) {
        for (int i = 0; i < options.size(); i++) {
            Rectangle opt = {area.x, area.y + (i + 1) * area.height, area.width, area.height};
            DrawRectangleRec(opt, RAYWHITE);
            DrawRectangleLinesEx(opt, 1, GRAY);
            DrawText(options[i].c_str(), opt.x + 8, opt.y + 8, 18, DARKGRAY);
        }
    }
}

string Dropdown::getSelectedOption() const {
    return options[selected];
}
