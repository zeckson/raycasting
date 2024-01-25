//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#ifndef RAYCASTING_MINIMAP_H
#define RAYCASTING_MINIMAP_H


#include <SDL_render.h>
#include "player.h"

#define GRID_SIZE 20

class Minimap {
public:
    explicit Minimap(const Player &player, int width, int height);

    void render(SDL_Renderer *pRenderer);

private:
    const Player& player;
    int width, height, size = GRID_SIZE;

    static void drawCircle(SDL_Renderer *pRenderer, int centerX, int centerY, int radius) ;

    void drawPlayer(SDL_Renderer *pRenderer) const;
};


#endif //RAYCASTING_MINIMAP_H
