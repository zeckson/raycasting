//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#include "minimap.h"
#include "map.h"
#include "base/renderer/pixel.h"
#include "base/renderer/pixelrenderer.h"

Minimap::Minimap(const Player &player, int width, int height) : player(player), width(width), height(height) {}

void Minimap::render(PixelRenderer *pRenderer) {
    if (!shown) return;
    SDL_Rect rect;
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            pRenderer->setColor(WHITE);
            int displayX = translateX(x);
            int displayY = translateY(y);
            rect = {displayX, displayY, cellSize, cellSize};
            pRenderer->drawRect(&rect);

            int wall = worldMap[x][y];
            if (wall) {
                Pixel color = colorMap[wall];
                pRenderer->setColor(color);
                rect = {displayX, displayY, cellSize, cellSize};
                pRenderer->drawRect(&rect);
            }
        }
    }
    drawPlayer(pRenderer);
}

int Minimap::translateY(int y) const { return y * cellSize; }

int Minimap::translateY(double y) const { return int(y * cellSize); }

int Minimap::translateX(int x) const { return x * cellSize; }

int Minimap::translateX(double x) const { return int(x * cellSize); }

void Minimap::drawPlayer(PixelRenderer *pRenderer) const {
    // Render player
    pRenderer->setColor(WHITE);
    int playerX = translateX(player.posX);
    int playerY = translateY(player.posY);
    pRenderer->drawCircle(playerX, playerY, cellSize / 2);

    // Render direction arrow
    pRenderer->setColor(BLUE);
    int directionX = translateX(player.posX + player.dirX);
    int directionY = translateY(player.posY + player.dirY);
    pRenderer->drawLine(playerX, playerY, directionX, directionY);

    // Render plane
    //calculate ray position and direction
    pRenderer->setColor(GREEN);
    double rayDirX = player.getRayDirX(-1);
    double rayDirY = player.getRayDirY(-1);
    directionX = translateX(player.posX + rayDirX);
    directionY = translateY(player.posY + rayDirY);
    pRenderer->drawLine(playerX, playerY, directionX, directionY);

    rayDirX = player.getRayDirX(1);
    rayDirY = player.getRayDirY(1);
    directionX = translateX(player.posX + rayDirX);
    directionY = translateY(player.posY + rayDirY);
    pRenderer->drawLine(playerX, playerY, directionX, directionY);

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

