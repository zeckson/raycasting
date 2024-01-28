//
// Created by Evgenii Shchepotev on 25.01.2024.
//

#include "engine.h"
#include "map.h"
#include "color.h"
#include "cmath"
#include "const.h"

Uint32 buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void Engine::render(SDL_Renderer *renderer) {
    for (int x = 0; x < width; x++) {
        double cameraX = 2 * x / (double) width - 1; //x-coordinate in camera space

        Intersection intersection = trace(cameraX);
        double perpWallDist = intersection.distance;

        int mapX = intersection.x;
        int mapY = intersection.y;
        const CellSide side = intersection.side;

        //Calculate height of line to draw on screen
        int lineHeight = (int) (height / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height) drawEnd = height - 1;
//        drawTexture(x, cameraX, perpWallDist, mapX, mapY, side, lineHeight, drawStart, drawEnd);

        drawColor(renderer, x, intersection, drawStart, drawEnd);
    }

//    drawBuffer(renderer);
}

void Engine::drawTexture(int x, double cameraX, double perpWallDist, int mapX, int mapY, const CellSide &side,
                         int lineHeight, int drawStart, int drawEnd) {//calculate value of wallX
    double wallX; //where exactly the wall was hit
    bool leftRight = side == CellSide::WEST || side == CellSide::EAST;
    if (leftRight) {
        wallX = player.posY + perpWallDist * player.getRayDirY(cameraX);
    } else {
        wallX = player.posX + perpWallDist * player.getRayDirX(cameraX);
    }
    wallX -= floor(wallX);

    //x coordinate on the texture
    int texX = int(wallX * double(texWidth));
    if(side == CellSide::WEST) texX = texWidth - texX - 1;
    if(side == CellSide::NORTH) texX = texWidth - texX - 1;

    int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!
    // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
    // How much to increase the texture coordinate per screen pixel
    double step = 1.0 * texHeight / lineHeight;
    // Starting texture coordinate
    double texPos = (drawStart - height / 2 + lineHeight / 2) * step;
    for(int y = drawStart; y < drawEnd; y++)
    {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
        const auto map = textureMap[texNum];
        u_int32_t color = map[texHeight * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(leftRight) color = (color >> 1) & 8355711;
        buffer[y][x] = color;
    }
}

[[maybe_unused]] void Engine::drawColor(SDL_Renderer *renderer, int x, const Intersection &intersection, int drawStart,
                  int drawEnd) const {
    int mapX = intersection.x;
    int mapY = intersection.y;

    const CellSide side = intersection.side;

    //choose wall color
    const int wallType = worldMap[mapX][mapY];

    Pixel color = colorMap[wallType];
    //give x and y sides different brightness
    if (side == CellSide::NORTH || side == CellSide::SOUTH) {
        color = color / 2;
    }

    // ...shade by distance...
    double distanceShade = 1.0 - fmin(intersection.distance / 24.0, 1.0);
    color = color * distanceShade;

    SDL_SetRenderColorRGB(renderer, color);
    // Draw the column
    //draw the pixels of the stripe as a vertical line
    SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
}

Intersection Engine::trace(double x) {
    //calculate ray position and direction
    double rayDirX = player.getRayDirX(x);
    double rayDirY = player.getRayDirY(x);

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

void Engine::drawBuffer(SDL_Renderer *renderer) const {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (texture == nullptr) {
        printf("Failed to create texture: %s\n", SDL_GetError());
    }

    // Update the texture with pixel data
    SDL_UpdateTexture(texture, nullptr, buffer, width * sizeof(Uint32));

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}
