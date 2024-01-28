//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#ifndef RAYCASTING_COLOR_H
#define RAYCASTING_COLOR_H

struct Pixel {
    u_int8_t r, g, b, a;

    Pixel(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    Pixel(int r, int g, int b) : Pixel(r, g, b, 255) {}

    Pixel operator/(const int value) const {
        return {u_int8_t(r / value), u_int8_t(g / value), u_int8_t(b / value), u_int8_t(a / value)};
    }
    Pixel operator*(const double value) const {
        return {u_int8_t(r * value), u_int8_t(g * value), u_int8_t(b * value), u_int8_t(a * value)};
    }
};

static const Pixel GREEN(0, 255, 0);
static const Pixel RED(255, 0, 0);
static const Pixel BLUE(0, 0, 255);
static const Pixel WHITE(255, 255, 255);
static const Pixel YELLOW(255, 255, 0);

static const Pixel colorMap[] = {
        WHITE, YELLOW, RED, GREEN, BLUE
};

inline void SDL_SetRenderColorRGB(SDL_Renderer *renderer, const Pixel &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif //RAYCASTING_COLOR_H
