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
public:
    explicit Engine(const Player &player, uint16_t width, uint16_t height) : player(player), width(width),
                                                                             height(height) {
    };

    void render(SDL_Renderer *renderer);

private:
    TextureMap textureMap;

    const Player &player;
    const uint16_t width, height;

    Intersection trace(double cameraX);

    [[maybe_unused]] void drawColor(SDL_Renderer *renderer, int x,
                   const Intersection &intersection,
                   int drawStart,
                   int drawEnd) const;

    void drawTexture(int x, double cameraX, double perpWallDist, int mapX, int mapY, const CellSide &side,
                     int lineHeight, int drawStart, int drawEnd);

    void drawBuffer(SDL_Renderer *renderer) const;
};


#endif //RAYCASTING_ENGINE_H
