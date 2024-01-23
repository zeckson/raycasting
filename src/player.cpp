//
// Created by Evgenii Shchepotev on 23.01.2024.
//

#include <cmath>
#include <SDL.h>
#include "player.h"
#include "map.h"

void Player::onKeyDown(const SDL_Keysym &key) {

    //speed modifiers
    double moveSpeed = 0.1 * MOVE_SPEED; //the constant value is in squares/second
    double rotSpeed = 0.1 * ROTATE_SPEED; //the constant value is in radians/second

    double move = 0;
    double rotate = 0;
    switch (key.scancode) {
        case SDL_SCANCODE_UP:
            move = +1;
            break;
        case SDL_SCANCODE_DOWN:
            move = -1;
            break;
        case SDL_SCANCODE_LEFT:
            rotate = rotSpeed;
            break;
        case SDL_SCANCODE_RIGHT:
            rotate = -rotSpeed;
            break;
        default:
            // do nothing
            break;
    }

    if (move != 0.0) {
        double deltaX = dirX * moveSpeed;
        double deltaY = dirY * moveSpeed;
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

Intersection Player::trace(double ray) {
    //calculate ray position and direction
    double rayDirX = dirX + planeX * ray;
    double rayDirY = dirY + planeY * ray;
    //which box of the map we're in
    int mapX = int(posX);
    int mapY = int(posY);

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //length of ray from one x or y-side to next x or y-side
    //these are derived as:
    //deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
    //deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
    //which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
    //where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
    //unlike (dirX, dirY) is not 1, however this does not matter, only the
    //ratio between deltaDistX and deltaDistY matters, due to the way the DDA
    //stepping further below works. So the values can be computed as below.
    // Division through zero is prevented, even though technically that's not
    // needed in C++ with IEEE 754 floating point values.
    double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

    double perpWallDist;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
    //calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }
    //perform DDA
    while (hit == 0) {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
    }
    //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
    //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
    //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
    //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
    //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
    //steps, but we subtract deltaDist once because one step more into the wall was taken above.
    if (side == 0) perpWallDist = (sideDistX - deltaDistX);
    else perpWallDist = (sideDistY - deltaDistY);

    return {mapX, mapY, perpWallDist};
}
