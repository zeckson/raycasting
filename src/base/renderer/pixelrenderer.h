//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#ifndef RAYCASTING_PIXELRENDERER_H
#define RAYCASTING_PIXELRENDERER_H

#include "SDL.h"
#include "pixel.h"

class PixelRenderer {
public:
    const Uint16 width, height;

    explicit PixelRenderer(SDL_Renderer *pRenderer, const Uint16 width, const Uint16 height)
            : width(width), height(height), pSDLRenderer(pRenderer) {

    }

    PixelRenderer *setColor(const Pixel &pixel);
    PixelRenderer *clear();
    PixelRenderer *render();

    void destroy();

private:
    SDL_Renderer *pSDLRenderer;
};


#endif //RAYCASTING_PIXELRENDERER_H
