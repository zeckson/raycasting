//
// Created by Evgenii Shchepotev on 25.01.2024.
//

#include "engine.h"
#include "player.h"
#include "map.h"
#include "color.h"
#include "cmath"

void Engine::render(SDL_Renderer *renderer) {
    for (int x = 0; x < width; x++) {
        Intersection intersection = trace(x);
        double perpWallDist = intersection.distance;

        //Calculate height of line to draw on screen
        int lineHeight = (int) (height / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height) drawEnd = height - 1;

        int mapX = intersection.x;
        int mapY = intersection.y;

        //choose wall color
        ColorRGB color = colorMap[worldMap[mapX][mapY]];

        //give x and y sides different brightness
        if (intersection.side == CellSide::NORTH || intersection.side == CellSide::SOUTH) {
            color = color / 2;
        }

        // ...shade by distance...
        double distanceShade = 1.0 - std::fmin(intersection.distance / 24.0, 1.0);
        color = color * distanceShade;

        SDL_SetRenderColorRGB(renderer, color);
        // Draw the column
        //draw the pixels of the stripe as a vertical line
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

Intersection Engine::trace(double x) {
    //calculate ray position and direction
    double cameraX = 2 * x / (double) width - 1; //x-coordinate in camera space

    //calculate ray position and direction
    double rayDirX = player.getRayDirX(cameraX);
    double rayDirY = player.getRayDirY(cameraX);

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
    double deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);

    //which box of the map we're in
    int mapX = int(player.posX);
    int mapY = int(player.posY);

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    //calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (player.posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (player.posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
    }

    //perform DDA
    int hit = 0; //was there a wall hit?
    CellSide side = CellSide::UNKNOWN; //was a NS or a EW wall hit?
    while (hit == 0) {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = stepX < 0 ? CellSide::EAST : CellSide::WEST;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = stepY < 0 ? CellSide::NORTH : CellSide::SOUTH;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
    }

    double perpWallDist;
    //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
    //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
    //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
    //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
    //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
    //steps, but we subtract deltaDist once because one step more into the wall was taken above.
    if (side == CellSide::EAST || side == CellSide::WEST) perpWallDist = (sideDistX - deltaDistX);
    else perpWallDist = (sideDistY - deltaDistY);

    return {mapX, mapY, perpWallDist, side};
}
