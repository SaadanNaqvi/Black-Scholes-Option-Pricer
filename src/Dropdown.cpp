#include "Dropdown.h"
#include "raylib.h"
#include <algorithm>

#define PANEL_COLOR   CLITERAL(Color){ 25, 40, 80, 255 }
#define HOVER_COLOR   CLITERAL(Color){ 45, 65, 120, 255 }
#define TEXT_COLOR    CLITERAL(Color){ 230, 230, 240, 255 }
#define ACCENT_COLOR  CLITERAL(Color){ 50, 150, 255, 255 }

void Dropdown::update() {
    Vector2 mouse = GetMousePosition();

    Rectangle clickable = {
        bounds.x - 3, bounds.y - 3,
        bounds.width + 6, bounds.height + 6
    };

    isHovered = CheckCollisionPointRec(mouse, clickable);

    // ---- Handle main toggle ----
    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
        return; // avoid processing in same frame
    }

    // ---- Handle scroll only when open ----
    if (isOpen && options.size() > visibleItems) {
        float scrollDelta = GetMouseWheelMove() * 35.0f;
        scrollOffset -= scrollDelta;
        float maxScroll = std::max(0.0f, (float)(options.size() - visibleItems) * bounds.height);
        scrollOffset = std::clamp(scrollOffset, 0.0f, maxScroll);
    }

    // ---- Handle option selection ----
    if (isOpen) {
        bool clickedInside = false;
        for (int i = 0; i < (int)options.size(); i++) {
            float y = bounds.y + bounds.height + (i * bounds.height) - scrollOffset;
            Rectangle itemRect = { bounds.x, y, bounds.width, bounds.height };

            // Skip items outside visible scroll area
            if (y + bounds.height < bounds.y + bounds.height ||
                y > bounds.y + bounds.height + (visibleItems * bounds.height))
                continue;

            if (CheckCollisionPointRec(mouse, itemRect)) {
                clickedInside = true;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedIndex = i;
                    isOpen = false;
                    scrollOffset = std::clamp(scrollOffset, 0.0f,
                        std::max(0.0f, (float)(options.size() - visibleItems) * bounds.height));
                    return;
                }
            }
        }

        // ---- Close dropdown if clicked outside visible area ----
        Rectangle expandedRect = {
            bounds.x,
            bounds.y + bounds.height,
            bounds.width,
            (float)(visibleItems * bounds.height)
        };
        if (!clickedInside &&
            !CheckCollisionPointRec(mouse, clickable) &&
            !CheckCollisionPointRec(mouse, expandedRect) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isOpen = false;
        }
    }
}



// Draw collapsed dropdown box (base)
void Dropdown::drawBase() {
    DrawRectangleRec(bounds, PANEL_COLOR);
    DrawRectangleLinesEx(bounds, 1.5f, ACCENT_COLOR);

    if (!options.empty())
        DrawText(options[selectedIndex].c_str(), bounds.x + 8, bounds.y + 8, 18, TEXT_COLOR);
    else
        DrawText("-", bounds.x + 8, bounds.y + 8, 18, TEXT_COLOR);

    // Dropdown arrow
    DrawTriangle(
        { bounds.x + bounds.width - 20, bounds.y + 15 },
        { bounds.x + bounds.width - 10, bounds.y + 15 },
        { bounds.x + bounds.width - 15, bounds.y + 25 },
        ACCENT_COLOR
    );
}

// Draw expanded dropdown ABOVE everything else
void Dropdown::drawExpanded() {
    if (!isOpen) return;

    bool scrollable = (options.size() > visibleItems);

    float totalHeight = (scrollable ? visibleItems : options.size()) * bounds.height;

    DrawRectangle(bounds.x, bounds.y + bounds.height,
                  bounds.width, totalHeight, PANEL_COLOR);
    DrawRectangleLines(bounds.x, bounds.y + bounds.height,
                       bounds.width, totalHeight, ACCENT_COLOR);

    for (int i = 0; i < options.size(); i++) {
        float y = bounds.y + bounds.height + i * bounds.height - scrollOffset;
        Rectangle itemRect = { bounds.x, y, bounds.width, bounds.height };

        // Only draw visible items
        if (y + bounds.height < bounds.y + bounds.height ||
            y > bounds.y + bounds.height + totalHeight)
            continue;

        bool hovered = CheckCollisionPointRec(GetMousePosition(), itemRect);
        DrawRectangleRec(itemRect, hovered ? HOVER_COLOR : PANEL_COLOR);
        DrawText(options[i].c_str(), itemRect.x + 8, itemRect.y + 8, 18, TEXT_COLOR);
    }

    // Scrollbar
    if (scrollable) {
        float scrollbarHeight = (visibleItems / (float)options.size()) * totalHeight;
        float scrollbarY = bounds.y + bounds.height +
            (scrollOffset / (options.size() * bounds.height)) * totalHeight;
        DrawRectangle(bounds.x + bounds.width - 5, scrollbarY, 4, scrollbarHeight, ACCENT_COLOR);
    }
}
