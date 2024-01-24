//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#ifndef RAYCASTING_COLOR_H
#define RAYCASTING_COLOR_H

struct ColorRGB {
    int r, g, b, a;

    ColorRGB(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    ColorRGB(int r, int g, int b) : ColorRGB(r, g, b, 255) {}

    ColorRGB operator/(int value) {
        return {r / value, g / value, b / value, a / value};
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

#endif //RAYCASTING_COLOR_H
