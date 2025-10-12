#ifndef INPUT_ELEMENT_H
#define INPUT_ELEMENT_H

#include "Graphics.h"

class InputElement : public Graphics {
protected:
    Rectangle area;
public:
    InputElement(Rectangle rect) : area(rect) {}
};

#endif
