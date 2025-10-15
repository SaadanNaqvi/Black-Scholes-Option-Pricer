#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "raylib.h"
#include <string>
#include <vector>
using namespace std;

class Dropdown {
private:
    Rectangle area;
    vector<string> options;
    int selected;
    bool open;

public:
    Dropdown(Rectangle rect, vector<string> opts, int defaultIdx = 0);

    void draw();
    void update();
    string getSelectedOption() const;
};

#endif
