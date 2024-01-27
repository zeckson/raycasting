//
// Created by Evgenii Shchepotev on 27.01.2024.
//

#ifndef RAYCASTING_UTIL_H
#define RAYCASTING_UTIL_H

#include "keyhandler.h"
#include "color.h"

inline void SDL_SetRenderPixel(SDL_Renderer *renderer, int x, int y, u_int32_t pixel) {
    ushort red = (pixel >> (0)) & 0xFF;
    ushort green = (pixel >> (8)) & 0xFF;
    ushort blue = (pixel >> (16)) & 0xFF;
    ushort alpha = (pixel >> (24)) & 0xFF;
    SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
    SDL_RenderDrawPoint(renderer, x, y);
}


#endif //RAYCASTING_UTIL_H
