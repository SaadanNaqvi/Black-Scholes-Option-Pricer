// Line.h
#ifndef LINE_H
#define LINE_H

#pragma once
#include "Graph.h"
#include <string>
#include <vector>
#include <algorithm>
#include "raylib.h"

class Line : public Graph {
private:
    std::vector<std::string> dates;
    std::vector<float> prices;

    float yMin = 0.0f;
    float yMax = 0.0f;
    int currentIndex = 0;

    int xTicks = 6;
    int yTicks = 6;

    bool animating = false;
    float timeAccumulator = 0.0f;

public:
    Line(Rectangle rect, std::vector<std::string> t, std::vector<float> p);
    void startAnimation();
    void draw() override;
    void simulation(float dt) override;
    void startAnimationFrom(int index);
    void stopAnimation();
    int getCurrentIndex() const;
    std::string getPausedDate() const;
    std::string getCurrentDate() const;
};


#endif
