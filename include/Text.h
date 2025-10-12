#ifndef TEXT_H
#define TEXT_H

#include "InputElement.h"
#include <string>
using namespace std;


class Text : public InputElement {
private:
    string placeholder;
public:
    Text(Rectangle rect, string text);
    void draw() override;
};

#endif