#ifndef ACAV_SDLRENDERER_H
#define ACAV_SDLRENDERER_H

#include <SDL.h>
#include "Map.h"

class SDLRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* mapTexture;
    int windowSize;
    int currentMapDim;

    static constexpr Uint32 COLOR_EMPTY     = 0xFF282828;
    static constexpr Uint32 COLOR_ROAD      = 0xFF505050;
    static constexpr Uint32 COLOR_BOUNDARY  = 0xFFC8C8C8;
    static constexpr Uint32 COLOR_EGO       = 0xFF00C864;
    static constexpr Uint32 COLOR_CPU       = 0xFFC83232;

    void recreateTexture(int mapDim);

public:
    explicit SDLRenderer(int guiSize);
    ~SDLRenderer();

    void draw(const Map& map);
    bool pollEvents();
};

#endif //ACAV_SDLRENDERER_H
