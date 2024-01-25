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
            int displayX = translateX(x);
            int displayY = translateY(y);
            rect = {displayX, displayY, size, size};
            SDL_RenderDrawRect(pRenderer, &rect);

            int wall = worldMap[x][y];
            if (wall) {
                ColorRGB color = colorMap[wall];
                SDL_SetRenderColorRGB(pRenderer, color);
                rect = {displayX, displayY, size, size};
                SDL_RenderDrawRect(pRenderer, &rect);
            }
        }
    }
    drawPlayer(pRenderer);
}

int Minimap::translateY(int y) const { return y * size; }
int Minimap::translateY(double y) const { return y * size; }

int Minimap::translateX(int x) const { return x * size; }
int Minimap::translateX(double x) const { return x * size; }

void Minimap::drawPlayer(SDL_Renderer *pRenderer) const {
    int playerX = translateX(player.posX);
    int playerY = translateY(player.posY);
    drawCircle(pRenderer, playerX, playerY, size / 2);
    SDL_SetRenderColorRGB(pRenderer, BLUE);
    int directionX = int(playerX + translateX(player.dirX));
    int directionY = int(playerY + translateY(player.dirY));
    SDL_RenderDrawLine(pRenderer, playerX, playerY, directionX, directionY);
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

