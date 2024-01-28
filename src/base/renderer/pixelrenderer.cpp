//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#include "pixelrenderer.h"

PixelRenderer *PixelRenderer::setColor(const Pixel &pixel) {
    SDL_SetRenderDrawColor(pSDLRenderer, pixel.r(), pixel.g(), pixel.b(), pixel.a());
    return this;
}

PixelRenderer *PixelRenderer::clear() {
    SDL_RenderClear(pSDLRenderer);
    return this;
}

PixelRenderer *PixelRenderer::render() {
    SDL_RenderPresent(pSDLRenderer);
    return this;
}

void PixelRenderer::destroy() {
    SDL_DestroyRenderer(pSDLRenderer);
}
