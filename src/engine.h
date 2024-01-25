//
// Created by Evgenii Shchepotev on 25.01.2024.
//

#ifndef RAYCASTING_ENGINE_H
#define RAYCASTING_ENGINE_H

#include "player.h"
#include "texturemap.h"
#include "SDL.h"

enum class CellSide {
    EAST = 0,
    WEST,
    NORTH,
    SOUTH,
    UNKNOWN = 99
};

struct Intersection {
    Intersection(int x, int y, double distance, CellSide side) : x(x), y(y), distance(distance), side(side) {}

    int x, y;
    double distance;
    CellSide side;
};

class Engine {
private:
    const TextureMap textureMap;

    const uint16_t width, height;

    Intersection trace(double x);

public:
    explicit Engine(uint16_t width, uint16_t height) : width(width), height(height) {};

    void render(SDL_Renderer *renderer);

    Player player;
};


#endif //RAYCASTING_ENGINE_H
