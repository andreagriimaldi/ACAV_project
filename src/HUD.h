#ifndef ACAV_HUD_H
#define ACAV_HUD_H

#include <SDL.h>

#include "EgoTelemetry.h"

// Vertical telemetry panel, smartphone aspect ratio.
// Every internal dimension derives from GUI_SIZE, so changing GUI_SIZE
// rescales the whole panel with no other edit.
class HUD {
public:
    int GUI_SIZE;                  // panel height, px
    static constexpr double GUI_ASPECT = 9.0 / 19.5;           // width / height
    int GUI_WIDTH;

    explicit HUD(int size): GUI_SIZE(size), GUI_WIDTH(static_cast<int>(GUI_SIZE * GUI_ASPECT)){};
    // Draws the panel with its top-left corner at (originX, originY).
    void render(SDL_Renderer* r, const EgoTelemetry& t, int originX, int originY) const;
    int getWidth() const;
private:
    // section heights as fractions of GUI_SIZE (must sum to 1.0)
    static constexpr double F_SPEED  = 0.30;
    static constexpr double F_ACC    = 0.23;
    static constexpr double F_OPT    = 0.23;
    static constexpr double F_ACTIVE = 0.24;

    static void glyph(SDL_Renderer*, char, int x, int y, int px);
    static void text (SDL_Renderer*, const char*, int x, int y, int px);
    static void textCentered(SDL_Renderer*, const char*, int cx, int y, int px);
    static int  textWidth(const char*, int px);
    static void frame(SDL_Renderer*, const SDL_Rect&);
    static void bar(SDL_Renderer*, const SDL_Rect&, double frac);
};

#endif //ACAV_HUD_H