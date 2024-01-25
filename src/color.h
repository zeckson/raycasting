//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#ifndef RAYCASTING_COLOR_H
#define RAYCASTING_COLOR_H

struct ColorRGB {
    u_int8_t r, g, b, a;

    ColorRGB(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    ColorRGB(int r, int g, int b) : ColorRGB(r, g, b, 255) {}

    ColorRGB operator/(const int value) {
        return {u_int8_t(r / value), u_int8_t(g / value), u_int8_t(b / value), u_int8_t(a / value)};
    }
    ColorRGB operator*(const double value) {
        return {u_int8_t(r * value), u_int8_t(g * value), u_int8_t(b * value), u_int8_t(a * value)};
    }
};

static const ColorRGB GREEN(0, 255, 0);
static const ColorRGB RED(255, 0, 0);
static const ColorRGB BLUE(0, 0, 255);
static const ColorRGB WHITE(255, 255, 255);
static const ColorRGB YELLOW(255, 255, 0);

static const ColorRGB colorMap[] = {
        WHITE, YELLOW, RED, GREEN, BLUE
};

inline void SDL_SetRenderColorRGB(SDL_Renderer *renderer, const ColorRGB &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif //RAYCASTING_COLOR_H
