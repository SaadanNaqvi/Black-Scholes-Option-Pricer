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

    int xTicks = 6;
    int yTicks = 6;
    float yMin = 0, yMax = 0;
    float timeAccumulator = 0.0f;
    int currentIndex = 0;
    bool animating = false;

public:
    Line(Rectangle rect, vector<string> t, vector<float> p);

    void draw() override;
    void simulation(float dt) override;

    void startAnimation();
    void stopAnimation();
    string getPausedDate() const;
    void startAnimationFrom(int index);

    int getCurrentIndex() { return currentIndex; }
    

    virtual ~Line() = default;
};

#endif
