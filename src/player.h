//
// Created by Evgenii Shchepotev on 23.01.2024.
//

#ifndef RAYCASTING_PLAYER_H
#define RAYCASTING_PLAYER_H

#define MOVE_SPEED 5.0
#define ROTATE_SPEED 3.0

#include <SDL_events.h>

class Player {
public:
    void onKeyDown(const SDL_Keysym &key);

    [[nodiscard]] double getRayDirX(const double cameraX) const { return dirX + planeX * cameraX; };
    [[nodiscard]] double getRayDirY(const double cameraX) const { return dirY + planeY * cameraX; };

    //x and y start position
    double posX = 22;
    double posY = 12;
    //initial direction vector
    double dirX = -1;
    double dirY = 0;
    //the 2d raycaster version of camera plane
    double planeX = 0;
    double planeY = 0.66;
};


#endif //RAYCASTING_PLAYER_H
