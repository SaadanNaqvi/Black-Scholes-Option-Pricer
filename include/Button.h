#ifndef BUTTON_H
#define BUTTON_H

#include "InputElement.h"
#include <string>
using namespace std;

// Class inherits from input element
class Button : public InputElement {
private:
    string label; // Text written inside button
    Rectangle bounds; // The buttons clicable area
public:
    Button(Rectangle rect, string label); // Button constructor

    // Handles the reading of the button background, outline and label
    void draw() override;

    bool isClicked(); // Detects if user has clicked button

    void setPosition(Vector2 pos); // Updates the buttons on screen position

    void setLabel(string newLabel);


};

#endif