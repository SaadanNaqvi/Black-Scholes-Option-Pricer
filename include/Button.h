#ifndef BUTTON_H
#define BUTTON_H

#include "InputElement.h"
#include <string>
using namespace std;


class Button : public InputElement {
private:
    string label;
public:
    Button(Rectangle rect, string label);
    void draw() override;
    bool isClicked();
};

#endif