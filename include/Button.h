#ifndef BUTTON_H
#define BUTTON_H

#include "InputElement.h"
#include <string>
using namespace std;


class Button : public InputElement {
private:
    string label;
    Rectangle bounds;
public:
    Button(Rectangle rect, string label);
    void draw() override;
    bool isClicked();
    void setPosition(Vector2 pos) { bounds.x = pos.x; bounds.y = pos.y; }

};

#endif