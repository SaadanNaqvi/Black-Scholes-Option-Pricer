#ifndef BAR_H
#define BAR_H
#include "Graph.h"
#include <vector>
#include "Theme.h"
using namespace std;

class Bar : public Graph {
private:
    // Stores the height/value of each bar (e.g., Cash, PnL, etc.)
    vector<float> values;
public:

    Bar(Rectangle rect, vector<float> vals); // Constructs a Bar graph object.

    // Called each frame from the main dashboard loop.
    // The draw() function uses the Theme colors (from Theme.h) and rectangle geometry
    void draw() override;

    //This function is optional for static bars but useful if you want bar heights
    //to transition smoothly over time during simulations.
    void simulation(float dt) override;

    // Virtual destructor for safe cleanup of derived graph types.
    virtual ~Bar() = default;
};
#endif
