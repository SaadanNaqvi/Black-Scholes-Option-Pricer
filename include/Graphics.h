#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>
#include <vector>
#include <memory>

class Graphics {
public:
    virtual void draw() {}
    virtual void update() {}

    // Dashboard that assembles everything visually
    void dashboard();
};

#endif
