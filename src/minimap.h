//
// Created by Evgenii Shchepotev on 24.01.2024.
//

#ifndef RAYCASTING_MINIMAP_H
#define RAYCASTING_MINIMAP_H


#include <SDL_render.h>
#include "player.h"
#include "base/keyhandler.h"
#include "base/renderer/pixelrenderer.h"

const short sizes[] = {2, 4, 8, 12, 20};

class Minimap: KeyHandler {
public:
    explicit Minimap(const Player &player, int width, int height);

    void render(PixelRenderer *pRenderer);

    void onKeyDown(const SDL_Keysym &key) override;
    void onKeyUp(const SDL_Keysym &key) override;
private:
    const Player& player;
    short zoom = 3;
    int width, height, cellSize = sizes[zoom];
    bool shown = false;

    void drawPlayer(PixelRenderer *pRenderer) const;

    [[nodiscard]] int translateX(int x) const;
    [[nodiscard]] int translateX(double x) const;

    [[nodiscard]] int translateY(int y) const;
    [[nodiscard]] int translateY(double y) const;

};


#endif //RAYCASTING_MINIMAP_H
