#include "HUD.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace {

// ---------------------------------------------------------------- font ----
// 5 wide x 7 tall, bit4 = leftmost pixel of the row.
struct Glyph { char c; unsigned char rows[7]; };

const Glyph FONT[] = {
    {' ', {0,0,0,0,0,0,0}},
    {'A', {0b01110,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001}},
    {'B', {0b11110,0b10001,0b10001,0b11110,0b10001,0b10001,0b11110}},
    {'C', {0b01110,0b10001,0b10000,0b10000,0b10000,0b10001,0b01110}},
    {'D', {0b11110,0b10001,0b10001,0b10001,0b10001,0b10001,0b11110}},
    {'E', {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b11111}},
    {'F', {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b10000}},
    {'G', {0b01110,0b10001,0b10000,0b10111,0b10001,0b10001,0b01111}},
    {'H', {0b10001,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001}},
    {'I', {0b01110,0b00100,0b00100,0b00100,0b00100,0b00100,0b01110}},
    {'J', {0b00111,0b00010,0b00010,0b00010,0b00010,0b10010,0b01100}},
    {'K', {0b10001,0b10010,0b10100,0b11000,0b10100,0b10010,0b10001}},
    {'L', {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b11111}},
    {'M', {0b10001,0b11011,0b10101,0b10101,0b10001,0b10001,0b10001}},
    {'N', {0b10001,0b11001,0b10101,0b10011,0b10001,0b10001,0b10001}},
    {'O', {0b01110,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110}},
    {'P', {0b11110,0b10001,0b10001,0b11110,0b10000,0b10000,0b10000}},
    {'Q', {0b01110,0b10001,0b10001,0b10001,0b10101,0b10010,0b01101}},
    {'R', {0b11110,0b10001,0b10001,0b11110,0b10100,0b10010,0b10001}},
    {'S', {0b01111,0b10000,0b10000,0b01110,0b00001,0b00001,0b11110}},
    {'T', {0b11111,0b00100,0b00100,0b00100,0b00100,0b00100,0b00100}},
    {'U', {0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110}},
    {'V', {0b10001,0b10001,0b10001,0b10001,0b10001,0b01010,0b00100}},
    {'W', {0b10001,0b10001,0b10001,0b10101,0b10101,0b11011,0b10001}},
    {'X', {0b10001,0b10001,0b01010,0b00100,0b01010,0b10001,0b10001}},
    {'Y', {0b10001,0b10001,0b01010,0b00100,0b00100,0b00100,0b00100}},
    {'Z', {0b11111,0b00001,0b00010,0b00100,0b01000,0b10000,0b11111}},
    {'0', {0b01110,0b10001,0b10011,0b10101,0b11001,0b10001,0b01110}},
    {'1', {0b00100,0b01100,0b00100,0b00100,0b00100,0b00100,0b01110}},
    {'2', {0b01110,0b10001,0b00001,0b00010,0b00100,0b01000,0b11111}},
    {'3', {0b11111,0b00010,0b00100,0b00010,0b00001,0b10001,0b01110}},
    {'4', {0b00010,0b00110,0b01010,0b10010,0b11111,0b00010,0b00010}},
    {'5', {0b11111,0b10000,0b11110,0b00001,0b00001,0b10001,0b01110}},
    {'6', {0b00110,0b01000,0b10000,0b11110,0b10001,0b10001,0b01110}},
    {'7', {0b11111,0b00001,0b00010,0b00100,0b01000,0b01000,0b01000}},
    {'8', {0b01110,0b10001,0b10001,0b01110,0b10001,0b10001,0b01110}},
    {'9', {0b01110,0b10001,0b10001,0b01111,0b00001,0b00010,0b01100}},
    {'.', {0b00000,0b00000,0b00000,0b00000,0b00000,0b01100,0b01100}},
    {':', {0b00000,0b01100,0b01100,0b00000,0b01100,0b01100,0b00000}},
    {'-', {0b00000,0b00000,0b00000,0b11111,0b00000,0b00000,0b00000}},
    {'/', {0b00001,0b00010,0b00010,0b00100,0b01000,0b01000,0b10000}},
    {'=', {0b00000,0b00000,0b11111,0b00000,0b11111,0b00000,0b00000}},
    {'%', {0b11001,0b11010,0b00010,0b00100,0b01000,0b01011,0b10011}},
    {'(', {0b00010,0b00100,0b01000,0b01000,0b01000,0b00100,0b00010}},
    {')', {0b01000,0b00100,0b00010,0b00010,0b00010,0b00100,0b01000}},
};

const unsigned char* lookup(char c) {
    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    for (const Glyph& g : FONT)
        if (g.c == c) return g.rows;
    return FONT[0].rows;
}

// ------------------------------------------------------------- palette ----
inline void colBG   (SDL_Renderer* r) { SDL_SetRenderDrawColor(r,  14,  16,  22, 255); }
inline void colFrame(SDL_Renderer* r) { SDL_SetRenderDrawColor(r,  70,  80,  96, 255); }
inline void colLabel(SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 130, 142, 160, 255); }
inline void colValue(SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 228, 234, 244, 255); }
inline void colGreen (SDL_Renderer* r){ SDL_SetRenderDrawColor(r,  46, 204,  96, 255); }
inline void colOff  (SDL_Renderer* r) { SDL_SetRenderDrawColor(r,  38,  42,  52, 255); }
inline void colTrack(SDL_Renderer* r) { SDL_SetRenderDrawColor(r,  52,  58,  70, 255); }
inline void colRedBG  (SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 110,  20,  26, 255); }
inline void colRedEdge(SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 235,  60,  60, 255); }
inline void colRedTxt (SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 255, 225, 225, 255); }

double frac01(double num, double den) {
    if (!std::isfinite(num) || den <= 0.0) return 0.0;
    return std::clamp(num / den, 0.0, 1.0);
}

inline void colAmber(SDL_Renderer* r) { SDL_SetRenderDrawColor(r, 235, 175,  50, 255); }
inline void colBlue (SDL_Renderer* r) { SDL_SetRenderDrawColor(r,  90, 160, 235, 255); }

const char* fsmName(FSM s) {
switch (s) {
    case FSM::NONE:            return "---";
    case FSM::NORMAL:          return "NORMAL";
    case FSM::REQUESTING_STOP: return "REQ STOP";
    case FSM::STOPPED:         return "STOPPED";
    case FSM::RESTART:         return "RESTART";
}
return "???";
}

void colFSM(SDL_Renderer* r, FSM s) {
switch (s) {
    case FSM::NONE:            colLabel(r);   return;
    case FSM::NORMAL:          colGreen(r);   return;
    case FSM::REQUESTING_STOP: colAmber(r);   return;
    case FSM::STOPPED:         colRedEdge(r); return;
    case FSM::RESTART:         colBlue(r);    return;
}
colLabel(r);
}

} // namespace

// -------------------------------------------------------------- glyphs ----
void HUD::glyph(SDL_Renderer* r, char c, int x, int y, int px) {
    const unsigned char* g = lookup(c);
    SDL_Rect p{0, 0, px, px};
    for (int row = 0; row < 7; ++row)
        for (int col = 0; col < 5; ++col) {
            if (!(g[row] & (1 << (4 - col)))) continue;
            p.x = x + col * px;
            p.y = y + row * px;
            SDL_RenderFillRect(r, &p);
        }
}

int HUD::textWidth(const char* s, int px) {
    const int n = static_cast<int>(std::strlen(s));
    return n > 0 ? (n * 6 - 1) * px : 0;
}

void HUD::text(SDL_Renderer* r, const char* s, int x, int y, int px) {
    for (int i = 0; s[i] != '\0'; ++i)
        glyph(r, s[i], x + i * 6 * px, y, px);
}

void HUD::textCentered(SDL_Renderer* r, const char* s, int cx, int y, int px) {
    text(r, s, cx - textWidth(s, px) / 2, y, px);
}

void HUD::frame(SDL_Renderer* r, const SDL_Rect& rect) {
    colFrame(r);
    SDL_RenderDrawRect(r, &rect);
}

void HUD::bar(SDL_Renderer* r, const SDL_Rect& rect, double f) {
    colTrack(r);
    SDL_RenderFillRect(r, &rect);
    const int w = static_cast<int>(std::clamp(f, 0.0, 1.0) * rect.w);
    if (w <= 0) return;
    SDL_Rect fill{rect.x, rect.y, w, rect.h};
    colValue(r);
    SDL_RenderFillRect(r, &fill);
}

int HUD::getWidth() const {
    return GUI_WIDTH;
}

// -------------------------------------------------------------- render ----
void HUD::render(SDL_Renderer* r, const EgoTelemetry& t, int originX, int originY) const {
    const int W = GUI_WIDTH;
    const int H = GUI_SIZE;

    // one font dot; sized so a 16-char line spans the usable width
    const int px  = std::max(1, W / (16 * 6));
    const int pad = std::max(2, H / 100);

    // background + outer border
    colBG(r);
    SDL_Rect bg{originX, originY, W, H};
    SDL_RenderFillRect(r, &bg);
    frame(r, bg);

    const int x0   = originX + pad;
    const int innW = W - 2 * pad;
    const int cx   = originX + W / 2;

    int y = originY + pad;
    char buf[48], nbuf[24];

    // ============================ 1. SPEED ================================
    {
        const int secH = static_cast<int>(F_SPEED * H);
        SDL_Rect sec{x0, y, innW, secH - pad};
        frame(r, sec);

        colLabel(r);
        text(r, "SPEED", sec.x + pad, sec.y + pad, px);

        // big "3.2/7.0"
        const int bigPx = std::max(px, static_cast<int>(secH / 35));
        std::snprintf(buf, sizeof buf, "%.1f/%.1f", t.getSpeed(), t.getMaxSpeed());
        colValue(r);
        textCentered(r, buf, cx, sec.y + (2 * secH) / 5 - 7 * bigPx / 2, bigPx);

        // fill bar, fraction of maxspeed
        SDL_Rect b{sec.x + pad, sec.y + sec.h - 4 * pad, sec.w - 2 * pad, 2 * pad};
        bar(r, b, frac01(t.getSpeed(), t.getMaxSpeed()));

        colLabel(r);
        std::snprintf(buf, sizeof buf, "%.0f%% VMAX",
                      100.0 * frac01(t.getSpeed(), t.getMaxSpeed()));
        textCentered(r, buf, cx, sec.y + sec.h - 8 * pad - 7 * px, px);

        y += secH;
    }

    // ============================= 2. ACC =================================
    {
        const int secH = static_cast<int>(F_ACC * H);
        SDL_Rect sec{x0, y, innW, secH - pad};

        if (t.getBraking()) {
            colRedBG(r);
            SDL_RenderFillRect(r, &sec);
            colRedEdge(r);
            SDL_RenderDrawRect(r, &sec);

            // two lines, sized to fit the longer word inside the section
            const int fitPx = (sec.w - 2 * pad) / (9 * 6 - 1);   // "EMERGENCY"
            const int ebPx  = std::max(px, std::min(2 * px, fitPx));
            const int blockH = 17 * ebPx;                        // 7 + gap 3 + 7
            const int top    = sec.y + secH / 2 - blockH / 2;

            colRedTxt(r);
            textCentered(r, "EMERGENCY", cx, top, ebPx);
            textCentered(r, "BRAKING",   cx, top + 10 * ebPx, ebPx);

            colRedTxt(r);
            text(r, "ACC", sec.x + pad, sec.y + pad, px);
        }
        else {
            frame(r, sec);

            colLabel(r);
            text(r, "ACC", sec.x + pad, sec.y + pad, px);

            if (t.getACCTracking()) {
                colLabel(r);
                text(r, "TRACKING", sec.x + sec.w - pad - textWidth("TRACKING", px),
                     sec.y + pad, px);

                const int midPx = std::max(px, static_cast<int>(secH / 20));
                std::snprintf(nbuf, sizeof nbuf, "%.2f", t.getACCSpeed());
                colValue(r);
                textCentered(r, nbuf, cx, sec.y + secH / 2 - 7 * midPx / 2, midPx);

                SDL_Rect b{sec.x + pad, sec.y + sec.h - 3 * pad, sec.w - 2 * pad, pad};
                bar(r, b, frac01(t.getACCSpeed(), t.getMaxSpeed()));
            } else {
                colLabel(r);
                textCentered(r, "NO TARGET", cx, sec.y + secH / 2 - 7 * px / 2, px);
            }
        }

        y += secH;
    }

    // ========================== 3. OPTIMIZER ==============================
    {
        const int secH = static_cast<int>(F_OPT * H);
        SDL_Rect sec{x0, y, innW, secH - pad};
        frame(r, sec);

        colLabel(r);
        text(r, "OPTIMIZER", sec.x + pad, sec.y + pad, px);

        // suggested speed, lifted to leave room for the state below
        const int midPx = std::max(px, static_cast<int>(secH / 30));
        std::snprintf(nbuf, sizeof nbuf, "%.2f", t.getOptimizerSpeed());
        colValue(r);
        textCentered(r, nbuf, cx, sec.y + (2 * secH) / 5 - 7 * midPx / 2, midPx);

        // FSM state, colour-coded, bottom of the section
        const char* sname = fsmName(t.getState());
        const int fitPx   = (sec.w - 2 * pad) / (8 * 6 - 1);   // "REQ STOP"
        const int stPx    = std::max(px, std::min(3 * px / 2, fitPx));

        const int mx = 4 * stPx;                    // horizontal margin per side
        const int my = 2 * stPx;                    // vertical margin per side

        const int bw = textWidth(sname, stPx) + 2 * mx;
        const int bh = 7 * stPx + 2 * my;
        SDL_Rect band{cx - bw / 2, sec.y + sec.h - 2 * pad - bh, bw, bh};
        colTrack(r);
        SDL_RenderFillRect(r, &band);

        colFSM(r, t.getState());
        textCentered(r, sname, cx, band.y + my, stPx);

        y += secH;
    }

    // ======================== 4. ACTIVE BRANCH ============================
    {
        const int secH = H - (y - originY) - pad;
        SDL_Rect sec{x0, y, innW, secH};
        frame(r, sec);

        colLabel(r);
        text(r, "ACTIVE", sec.x + pad, sec.y + pad, px);

        const int boxY = sec.y + 4 * pad + 7 * px;
        const int boxH = sec.h - (boxY - sec.y) - 2 * pad;
        const int boxW = (sec.w - 3 * pad) / 2;

        const int choice = t.getSpeedChoice();   // 0 physics, 1 ACC, 2 optimizer

        const char* labels[2] = {"ACC", "OPT"};
        for (int i = 0; i < 2; ++i) {
            SDL_Rect box{sec.x + pad + i * (boxW + pad), boxY, boxW, boxH};
            const bool on = (choice == i + 1);
            on ? colGreen(r) : colOff(r);
            SDL_RenderFillRect(r, &box);
            frame(r, box);

            if (on) SDL_SetRenderDrawColor(r, 10, 30, 14, 255);
            else    colLabel(r);
            textCentered(r, labels[i], box.x + box.w / 2,
                         box.y + box.h / 2 - 7 * px / 2, px);
        }

        if (choice == 0) {
            colLabel(r);
            text(r, "PHYSICS", sec.x + sec.w - pad - textWidth("PHYSICS", px),
                 sec.y + pad, px);
        }
    }
}