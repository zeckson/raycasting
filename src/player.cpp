//
// Created by Evgenii Shchepotev on 23.01.2024.
//

#include "player.h"
#include "map.h"
#include <cmath>
#include <SDL.h>

void Player::onKeyDown(const SDL_Keysym &key) {

    //speed modifiers
    double moveSpeed = 0.1 * MOVE_SPEED; //the constant value is in squares/second
    double rotSpeed = 0.1 * ROTATE_SPEED; //the constant value is in radians/second

    double move = 0;
    double rotate = 0;
    switch (key.scancode) {
        case SDL_SCANCODE_UP:
            move = moveSpeed;
            break;
        case SDL_SCANCODE_DOWN:
            move = -moveSpeed;
            break;
        case SDL_SCANCODE_LEFT:
            rotate = -rotSpeed;
            break;
        case SDL_SCANCODE_RIGHT:
            rotate = rotSpeed;
            break;
        default:
            // do nothing
            break;
    }

    if (move != 0) {
        double deltaX = dirX * move;
        double deltaY = dirY * move;
        if(worldMap[int(posX + deltaX)][int(posY)] == false) posX += deltaX;
        if(worldMap[int(posX)][int(posY + deltaY)] == false) posY += deltaY;

        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                       "Move to: (%f, %f)", posX, posY);
    }

    if (rotate != 0) {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotate) - dirY * sin(rotate);
        dirY = oldDirX * sin(rotate) + dirY * cos(rotate);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotate) - planeY * sin(rotate);
        planeY = oldPlaneX * sin(rotate) + planeY * cos(rotSpeed);
    }

}

Player::Player(double posX, double posY, double yawRad): posX(posX), posY(posY) {
    //initial direction vector
    dirX = cos(yawRad);
    dirY = sin(yawRad);
    //the 2d raycaster version of camera plane
    planeX = PLANE_X_RADIUS * cos(yawRad + M_PI_2);
    planeY = PLANE_X_RADIUS * sin(yawRad + M_PI_2);
}

