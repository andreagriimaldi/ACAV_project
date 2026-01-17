#ifndef ACAV_SDLRENDERER_H
#define ACAV_SDLRENDERER_H

#include <SDL.h>
#include "Map.h"

class SDLRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowSize;
    double scale;

    // Colors
    struct Color { Uint8 r, g, b; };
    static constexpr Color COLOR_EMPTY     = {40, 40, 40};
    static constexpr Color COLOR_ROAD      = {80, 80, 80};
    static constexpr Color COLOR_BOUNDARY  = {200, 200, 200};
    static constexpr Color COLOR_EGO       = {0, 200, 100};
    static constexpr Color COLOR_CPU       = {200, 50, 50};

    void drawPoint(int x, int y, Color c);

public:
    explicit SDLRenderer(int guiSize);
    ~SDLRenderer();

    void draw(const Map& map);
    bool pollEvents();
};

#endif //ACAV_SDLRENDERER_H
