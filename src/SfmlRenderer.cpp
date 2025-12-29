#include "SfmlRenderer.h"

sf::Color SfmlRenderer::colorFromPoint(const Point &point) const {
    if (point.occupied())
        return sf::Color::Red;

    switch (point.getType()) {
        case Point_type::Empty:
            return sf::Color::Green;
        case Point_type::Boundary:
            return sf::Color::White;
        case Point_type::Road:
            return sf::Color::Black;
        case Point_type::Rotation:
            return sf::Color::Magenta;
    }

    return sf::Color::Cyan;
}

void SfmlRenderer::draw(const Map &map) {
    window.clear();

    const auto& grid = map.getGrid();
    for (int i = 0; i < map.getDim() + 1; i++) {
        for (int j = 0; j < map.getDim() + 1; j++) {
            sf::RectangleShape cell({cellSize, cellSize});
            cell.setPosition(sf::Vector2f{
                static_cast<float>(j) * cellSize,
                static_cast<float>(i) * cellSize
            });

            cell.setFillColor(colorFromPoint(grid[i][j]));
            window.draw(cell);
        }
    }
    window.display();
}
