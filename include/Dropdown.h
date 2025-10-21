#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class Dropdown {
private:
    Rectangle bounds;
    std::vector<std::string> options;
    int selectedIndex;
    bool isOpen;
    float scrollOffset;
    int visibleItems;
    bool isHovered;

public:
    Dropdown(Rectangle rect, vector<std::string> opts)
        : bounds(rect), options(opts), selectedIndex(0), isOpen(false),
          scrollOffset(0), visibleItems(5), isHovered(false) {}

    void update();
    void drawBase();       
    void drawExpanded();   
    string getSelectedOption() const {
        if (options.empty()) return "";
        return options[selectedIndex];
    }

    bool getIsOpen() const { return isOpen; }
    void close() { isOpen = false; }
    Rectangle getBounds() const { return bounds; }
    
};

#endif
