// Line.h
#ifndef LINE_H
#define LINE_H
#include "Graph.h"
#include "Theme.h"
#include <vector>
#include <string>
using namespace std;

class Line : public Graph {
private:
    vector<float> prices;
    vector<string> dates;
    float simulationProgress = 0.0f;

    int xTicks = 6;
    int yTicks = 6;
    float yMin = 0, yMax = 0;

public:
    Line(Rectangle rect, vector<std::string> t, vector<float> p);

    void draw() override;
    void simulation(float dt) override;
};

#endif
