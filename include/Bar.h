#ifndef BAR_H
#define BAR_H

#include "Graph.h"
#include <vector>
using namespace std;

class Bar : public Graph {
private:
    vector<float> values;
public:
    Bar(Rectangle rect, vector<float> vals);
    void draw() override;
};

#endif
