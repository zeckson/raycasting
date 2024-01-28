//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#include "minimap.h"
#include "map.h"
#include "color.h"

Minimap::Minimap(const Player &player, int width, int height) : player(player), width(width), height(height) {}

void Minimap::render(SDL_Renderer *pRenderer) {
    if (!shown) return;
    SDL_Rect rect;
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            SDL_SetRenderColorRGB(pRenderer, WHITE);
            int displayX = translateX(x);
            int displayY = translateY(y);
            rect = {displayX, displayY, cellSize, cellSize};
            SDL_RenderDrawRect(pRenderer, &rect);

            int wall = worldMap[x][y];
            if (wall) {
                Pixel color = colorMap[wall];
                SDL_SetRenderColorRGB(pRenderer, color);
                rect = {displayX, displayY, cellSize, cellSize};
                SDL_RenderDrawRect(pRenderer, &rect);
            }
        }
    }
    drawPlayer(pRenderer);
}

int Minimap::translateY(int y) const { return y * cellSize; }

int Minimap::translateY(double y) const { return int(y * cellSize); }

int Minimap::translateX(int x) const { return x * cellSize; }

int Minimap::translateX(double x) const { return int(x * cellSize); }

void Minimap::drawPlayer(SDL_Renderer *pRenderer) const {
    // Render player
    SDL_SetRenderColorRGB(pRenderer, WHITE);
    int playerX = translateX(player.posX);
    int playerY = translateY(player.posY);
    drawCircle(pRenderer, playerX, playerY, cellSize / 2);

    // Render direction arrow
    SDL_SetRenderColorRGB(pRenderer, BLUE);
    int directionX = translateX(player.posX + player.dirX);
    int directionY = translateY(player.posY + player.dirY);
    SDL_RenderDrawLine(pRenderer, playerX, playerY, directionX, directionY);

    // Render plane
    //calculate ray position and direction
    SDL_SetRenderColorRGB(pRenderer, GREEN);
    double rayDirX = player.getRayDirX(-1);
    double rayDirY = player.getRayDirY(-1);
    directionX = translateX(player.posX + rayDirX);
    directionY = translateY(player.posY + rayDirY);
    SDL_RenderDrawLine(pRenderer, playerX, playerY, directionX, directionY);

    rayDirX = player.getRayDirX(1);
    rayDirY = player.getRayDirY(1);
    directionX = translateX(player.posX + rayDirX);
    directionY = translateY(player.posY + rayDirY);
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

void Minimap::onKeyDown(const SDL_Keysym &key) {
    if (key.sym == SDLK_TAB) {
        shown = !shown;
    }

    if (key.sym == SDLK_EQUALS) {
        zoom++;
    }
    if (key.sym == SDLK_MINUS) {
        zoom--;
    }

    if (zoom < 0) {
        zoom = 0;
    }
    auto length = short(sizeof(sizes) / sizeof(short));
    if (zoom >= length) {
        zoom = length - 1;
    }

    cellSize = sizes[zoom];
}

void Minimap::onKeyUp(const SDL_Keysym &key) {

}

