// Line.h
#ifndef LINE_H
#define LINE_H

#pragma once
#include "Graph.h"
#include <string>
#include <vector>
#include <algorithm>
#include "raylib.h"

/**
 *
 * The Line class visualizes time-series data — typically stock prices — and supports
 * dynamic animation (used in simulations). It draws smooth line plots of price movement
 * and allows pausing/resuming at any time index.
 *
 * Used in the dashboard to display:
 *  - Stock price history
 *  - Simulation progress
 *  - Contract start and end visualization
 */
class Line : public Graph {
private:
    /// List of date strings corresponding to each price point (e.g., "2023-06-15")
    std::vector<std::string> dates;

    /// Historical stock prices matching the dates above
    std::vector<float> prices;

    /// Minimum Y-axis value (for scaling graph vertically)
    float yMin = 0.0f;

    /// Maximum Y-axis value (for scaling graph vertically)
    float yMax = 0.0f;

    /// Current index of the animation or simulation (points to current day)
    int currentIndex = 0;

    /// Number of tick marks on the X-axis (date labels)
    int xTicks = 6;

    /// Number of tick marks on the Y-axis (price levels)
    int yTicks = 6;

    /// Whether the graph is currently animating (used for simulation mode)
    bool animating = false;

    /// Tracks elapsed time between animation frames (used for controlling playback speed)
    float timeAccumulator = 0.0f;

public:
    Line(Rectangle rect, std::vector<std::string> t, std::vector<float> p);

    void startAnimation(); // starts line animation

    void draw() override;

    void simulation(float dt) override;

    /**
     * @brief Starts the animation from a specific data index.
     * @param index The index to begin from (e.g., resume after pause).
     *
     * Useful for continuing simulations from where they were stopped.
     */
    void startAnimationFrom(int index);

    /**
     * @brief Stops the animation and freezes at the current index.
     *
     * Used when the user clicks “Stop” in the dashboard.
     */
    void stopAnimation();

    int getCurrentIndex() const;

    float getCurrentPrice() const;

    std::string getPausedDate() const;

    std::string getCurrentDate() const;
};

#endif // LINE_H
