#ifndef INPUT_ELEMENT_H
#define INPUT_ELEMENT_H

#include "Graphics.h"

class InputElement : public Graphics {
    protected:
        Rectangle area; // area of the input element
    public:
        InputElement(Rectangle rect) : area(rect) {} // input element constructor
};

#endif
