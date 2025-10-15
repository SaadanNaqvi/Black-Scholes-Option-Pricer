#ifndef GRAPH_H
#define GRAPH_H

#include "Graphics.h"

class Graph : public Graphics {
protected:
    Rectangle area;
public:

    Graph(Rectangle rect) : area(rect) {}
    virtual void simulation(float dt) override {}
};

#endif
