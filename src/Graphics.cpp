#include "Graphics.h"
#include "Graph.h"
#include "Line.h"
#include "Bar.h"
#include "Button.h"
#include "Text.h"
#include "InputElement.h"
#include <memory>
using namespace std;

void Graphics::dashboard() {
    vector<Vector2> stockData = {{80, 400}, {160, 370}, {240, 350}, {320, 320}, {400, 330}, {480, 310}};
    Line line(Rectangle{50, 100, 450, 250}, stockData);

    vector<float> profitLoss = {-700, 780, 10};
    Bar bars(Rectangle{550, 100, 300, 250}, profitLoss);

    Button exec(Rectangle{100, 500, 120, 40}, "Execute");
    Text option(Rectangle{260, 500, 150, 40}, "Option Type");

    // Draw all components
    line.draw();
    bars.draw();
    exec.draw();
    option.draw();

    DrawText("Portfolio Dashboard", 320, 30, 24, DARKBLUE);
}
