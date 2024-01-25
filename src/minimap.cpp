//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#include "minimap.h"
#include "map.h"
#include "color.h"

Minimap::Minimap(const Player &player, int width, int height) : player(player), width(width), height(height) {}

void Minimap::render(SDL_Renderer *pRenderer) {
    SDL_Rect rect;
    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            SDL_SetRenderColorRGB(pRenderer, WHITE);
            int displayX = x * size;
            int displayY = y * size;
            rect = {displayX, displayY, size, size};
            SDL_RenderDrawRect(pRenderer, &rect);

            int wall = worldMap[x][y];
            if (wall) {
                ColorRGB color = colorMap[wall];
                SDL_SetRenderColorRGB(pRenderer, color);
                for (int i = 0; i < size; ++i) {
                    rect = {displayX, displayY, size, size};
                    SDL_RenderDrawRect(pRenderer, &rect);
                }
            }
        }
    }
    drawPlayer(pRenderer);
}

void Minimap::drawPlayer(SDL_Renderer *pRenderer) const {
    int playerX = int(player.posX * size);
    int playerY = int(player.posY * size);
    drawCircle(pRenderer, playerX, playerY, size / 2);

}

void Minimap::drawCircle(SDL_Renderer *pRenderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(pRenderer, centerX + x, centerY + y);
            }
        }
    }
}
