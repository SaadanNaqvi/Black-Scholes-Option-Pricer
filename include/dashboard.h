#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <SFML/Graphics.hpp>

class Dashboard {
private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    sf::CircleShape  shape;

public:
    Dashboard();
    void run();
};

#endif
