#include "SDLRenderer.h"
#include <stdexcept>
#include <vector>

SDLRenderer::SDLRenderer(int guiSize)
    : windowSize(guiSize), mapTexture(nullptr), currentMapDim(0) {

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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

SDLRenderer::~SDLRenderer() {
    if (mapTexture) SDL_DestroyTexture(mapTexture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLRenderer::recreateTexture(int mapDim) {
    if (mapTexture) {
        SDL_DestroyTexture(mapTexture);
    }

    mapTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        mapDim,
        mapDim
    );

    if (!mapTexture) {
        throw std::runtime_error(std::string("Texture creation failed: ") + SDL_GetError());
    }

    currentMapDim = mapDim;
}

void SDLRenderer::draw(const Map& map) {
    int mapDim = map.getDim() + 1;

    if (mapDim != currentMapDim) {
        recreateTexture(mapDim);
    }

    const auto& grid = map.getGrid();

    // Lock texture for direct pixel access
    void* pixels;
    int pitch;
    if (SDL_LockTexture(mapTexture, nullptr, &pixels, &pitch) < 0) {
        return;
    }

    Uint32* pixelData = static_cast<Uint32*>(pixels);
    int pixelPitch = pitch / sizeof(Uint32);

    for (int y = 0; y < mapDim; y++) {
        for (int x = 0; x < mapDim; x++) {
            const auto& point = grid[x][y];
            Uint32 color;

            if (point->occupied()) {
                color = (point->getVehicle() == "ego") ? COLOR_EGO : COLOR_CPU;
            } else {
                switch (point->getType()) {
                    case Point_type::Empty:    color = COLOR_EMPTY;    break;
                    case Point_type::Road:     color = COLOR_ROAD;     break;
                    case Point_type::Boundary: color = COLOR_BOUNDARY; break;
                    case Point_type::Rotation: color = COLOR_ROAD;     break;
                    default:                   color = COLOR_EMPTY;    break;
                }
            }

            pixelData[y * pixelPitch + x] = color;
        }
    }

    SDL_UnlockTexture(mapTexture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr);

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
