//
// Created by Evgenii Shchepotev on 27.01.2024.
//

#ifndef RAYCASTING_KEYHANDLER_H
#define RAYCASTING_KEYHANDLER_H

#include "SDL.h"

class KeyHandler {
    public:
        virtual void onKeyDown(const SDL_Keysym &key) = 0;
        virtual void onKeyUp(const SDL_Keysym &key) = 0;
};


#endif //RAYCASTING_KEYHANDLER_H
