#include "DatePicker.h"
#include "raylib.h"
#include <algorithm>

#define PANEL_COLOR  CLITERAL(Color){ 25, 40, 80, 255 }
#define HOVER_COLOR  CLITERAL(Color){ 45, 65, 120, 255 }
#define TEXT_COLOR   CLITERAL(Color){ 230, 230, 240, 255 }
#define ACCENT_COLOR CLITERAL(Color){ 50, 150, 255, 255 }

DatePicker::DatePicker(Rectangle rect, std::vector<std::string> dates)
    : bounds(rect), availableDates(dates) {}

void DatePicker::setAvailableDates(vector<string> dates) {
    availableDates = dates;
    selectedIndex = -1;
    scrollIndex = 0;
}

void DatePicker::update() {
    Vector2 mouse = GetMousePosition();
    Rectangle clickable = { bounds.x - 3, bounds.y - 3, bounds.width + 6, bounds.height + 6 };

    if (CheckCollisionPointRec(mouse, clickable) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
        hasFocus = true;
    }

    if (isOpen) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < visibleItems && scrollIndex + i < availableDates.size(); i++) {
                float y = bounds.y + bounds.height + i * bounds.height;
                Rectangle itemRect = { bounds.x, y, bounds.width, bounds.height };
                if (CheckCollisionPointRec(mouse, itemRect)) {
                    selectedIndex = scrollIndex + i;
                    isOpen = false;
                    hasFocus = false;
                    return;
                }
            }
        }

        float scrollMove = GetMouseWheelMove();
        if (scrollMove != 0 && availableDates.size() > visibleItems) {
            scrollIndex -= scrollMove;
            scrollIndex = std::clamp(scrollIndex, 0, (int)availableDates.size() - visibleItems);
        }
    }

    // Close if clicked outside
    if (hasFocus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        !CheckCollisionPointRec(mouse, clickable) &&
        !(isOpen && CheckCollisionPointRec(mouse, {bounds.x, bounds.y + bounds.height, bounds.width, bounds.height * visibleItems}))) {
        isOpen = false;
        hasFocus = false;
    }
}

void DatePicker::drawBase() {
    DrawRectangleRec(bounds, PANEL_COLOR);
    DrawRectangleLinesEx(bounds, 1.5f, ACCENT_COLOR);

    if (selectedIndex >= 0 && selectedIndex < availableDates.size())
        DrawText(availableDates[selectedIndex].c_str(), bounds.x + 8, bounds.y + 8, 18, TEXT_COLOR);
    else
        DrawText("Select date", bounds.x + 8, bounds.y + 8, 18, TEXT_COLOR);

    DrawTriangle(
        { bounds.x + bounds.width - 20, bounds.y + 15 },
        { bounds.x + bounds.width - 10, bounds.y + 15 },
        { bounds.x + bounds.width - 15, bounds.y + 25 },
        ACCENT_COLOR
    );
}

void DatePicker::drawExpanded() {
    if (!isOpen) return;

    int displayCount = std::min(visibleItems, (int)availableDates.size());
    DrawRectangle(bounds.x, bounds.y + bounds.height, bounds.width, displayCount * bounds.height, PANEL_COLOR);
    DrawRectangleLines(bounds.x, bounds.y + bounds.height, bounds.width, displayCount * bounds.height, ACCENT_COLOR);

    for (int i = 0; i < displayCount && scrollIndex + i < availableDates.size(); i++) {
        float y = bounds.y + bounds.height + i * bounds.height;
        Rectangle itemRect = { bounds.x, y, bounds.width, bounds.height };

        bool hovered = CheckCollisionPointRec(GetMousePosition(), itemRect);
        DrawRectangleRec(itemRect, hovered ? HOVER_COLOR : PANEL_COLOR);

        DrawText(availableDates[scrollIndex + i].c_str(), itemRect.x + 8, itemRect.y + 8, 18, TEXT_COLOR);
    }

    if (availableDates.size() > visibleItems) {
        float barHeight = (visibleItems / (float)availableDates.size()) * (displayCount * bounds.height);
        float barY = bounds.y + bounds.height + (scrollIndex / (float)availableDates.size()) * (displayCount * bounds.height);
        DrawRectangle(bounds.x + bounds.width - 5, barY, 4, barHeight, ACCENT_COLOR);
    }
}

std::string DatePicker::getSelectedDate() const {
    if (selectedIndex >= 0 && selectedIndex < availableDates.size())
        return availableDates[selectedIndex];
    return "";
}
