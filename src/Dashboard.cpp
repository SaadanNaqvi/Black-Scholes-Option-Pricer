#include "Dashboard.h"

Dashboard::Dashboard() : window(sf::VideoMode{sf::Vector2u{800u, 600u}}, "Dashboard") , shape(50.f)
{
    shape.setFillColor(sf::Color::Blue);
    window.setFramerateLimit(60);
}

void Dashboard::run(){
    while (window.isOpen()){
        processEvents();
        update();
        render();
    }
}

void Dashboard::processEvents(){
    while (auto ev = window.pollEvent()){
        if (ev->is<sf::Event::Closed>()) window.close();
        if (const auto* key = ev->getIf<sf::Event::KeyPressed>()){
            if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
        }
    }
}

void Dashboard::update(){}
void Dashboard::render(){
    window.clear();
    window.draw(shape);
    window.display();
}
