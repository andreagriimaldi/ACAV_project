#include "SDLRenderer.h"
#include <stdexcept>
#include <cmath>

SDLRenderer::SDLRenderer(int guiSize) : windowSize(guiSize), scale(1.0) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("SDL init failed: ") + SDL_GetError());
    }

    window = SDL_CreateWindow(
        "ACAV",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowSize,
        windowSize,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Quit();
        throw std::runtime_error(std::string("Window creation failed: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(std::string("Renderer creation failed: ") + SDL_GetError());
    }
}

SDLRenderer::~SDLRenderer() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLRenderer::drawPoint(int x, int y, Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);

    int px1 = static_cast<int>(std::floor(x * scale));
    int py1 = static_cast<int>(std::floor(y * scale));
    int px2 = static_cast<int>(std::ceil((x + 1) * scale));
    int py2 = static_cast<int>(std::ceil((y + 1) * scale));

    SDL_Rect rect = {px1, py1, px2 - px1, py2 - py1};
    SDL_RenderFillRect(renderer, &rect);
}

void SDLRenderer::draw(const Map& map) {
    int mapDim = map.getDim() + 1;
    scale = static_cast<double>(windowSize) / static_cast<double>(mapDim);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const auto& grid = map.getGrid();

    for (int x = 0; x < mapDim; x++) {
        for (int y = 0; y < mapDim; y++) {
            const auto& point = grid[x][y];
            Color c;

            if (point->occupied()) {
                c = (point->getVehicle() == "ego") ? COLOR_EGO : COLOR_CPU;
            } else {
                switch (point->getType()) {
                    case Point_type::Empty:    c = COLOR_EMPTY;    break;
                    case Point_type::Road:     c = COLOR_ROAD;     break;
                    case Point_type::Boundary: c = COLOR_BOUNDARY; break;
                    case Point_type::Rotation: c = COLOR_ROAD;     break;
                }
            }

            drawPoint(x, y, c);
        }
    }

    SDL_RenderPresent(renderer);
}

bool SDLRenderer::pollEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}
