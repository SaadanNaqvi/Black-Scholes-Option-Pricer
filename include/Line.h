#ifndef LINE_H
#define LINE_H

#include "Graph.h"
#include <vector>
using namespace std;

class Line : public Graph {
private:
    vector<Vector2> points;
public:
    Line(Rectangle rect, vector<Vector2> pts);
    void draw() override;
};

#endif
