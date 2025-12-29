#ifndef ACAV_SFMLRENDERER_H
#define ACAV_SFMLRENDERER_H
#include <SFML/Graphics.hpp>
#include "Map.h"

class SfmlRenderer{
private:
    sf::RenderWindow window;
    float cellSize;
public:
    SfmlRenderer(int dim)
    : window(sf::VideoMode(sf::Vector2u{800, 800}),"Simulation"), cellSize(800.f / dim) {}

    sf::Color colorFromPoint(const Point & point) const;

    void draw(const Map& map);
};



#endif //ACAV_SFMLRENDERER_H