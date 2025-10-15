#ifndef LINE_H
#define LINE_H

#include "Graph.h"
#include <vector>
using namespace std;

class Line : public Graph {
private:
    vector<Vector2> points;
    float simulationProgress = 0.0f;
public:
    Line(Rectangle rect, vector<Vector2> pts);
    void draw() override;
    void simulation(float dt) override;
};

#endif
