#ifndef DATEPICKER_H
#define DATEPICKER_H

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class DatePicker {
private:
    Rectangle bounds;
    vector<string> availableDates;
    bool isOpen = false;
    int selectedIndex = -1;
    int scrollIndex = 0;
    int visibleItems = 7;
    bool hasFocus = false;

public:
    DatePicker(Rectangle rect, vector<string> dates);

    void update();
    void drawBase();
    void drawExpanded();

    void close() { isOpen = false; }
    bool getIsOpen() const { return isOpen; }

    std::string getSelectedDate() const;
    void setAvailableDates(const vector<string> dates);

    int getSelectedIndex() const { return selectedIndex; }
};

#endif
