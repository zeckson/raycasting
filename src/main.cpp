#include <iostream>
#include <cmath>
#include <vector>
#include <SDL.h>
#include "map.h"

const int screenWidth = 800;
const int screenHeight = 600;

enum class ColorRGB {
    RED,
    GREEN,
    BLUE,
    WHITE,
    YELLOW
};


int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame

    int w = screenWidth;
    int h = screenHeight;

    bool quit = false;
    SDL_Event e;

    while (!quit) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int x = 0; x < w; x++) {
            //calculate ray position and direction
            double cameraX = 2 * x / (double) w - 1; //x-coordinate in camera space
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;
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

            //Calculate height of line to draw on screen
            int lineHeight = (int) (h / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h) drawEnd = h - 1;

            //choose wall color
            ColorRGB color;
            switch (worldMap[mapX][mapY]) {
                case 1:
                    color = ColorRGB::RED;
                    break; //red
                case 2:
                    color = ColorRGB::GREEN;
                    break; //green
                case 3:
                    color = ColorRGB::BLUE;
                    break; //blue
                case 4:
                    color = ColorRGB::WHITE;
                    break; //white
                default:
                    color = ColorRGB::YELLOW;
                    break; //yellow
            }

            //give x and y sides different brightness
//            if (side == 1) { color = color / 2; }


            // Draw the column
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            SDL_RenderDrawLine(renderer, x, drawStart, x,
                               drawEnd);//draw the pixels of the stripe as a vertical line

        }

        //timing for input and FPS counter
        oldTime = time;
        time = SDL_GetTicks();
        double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
//        print(1.0 / frameTime); //FPS counter
//        redraw();
//        cls();

        SDL_RenderPresent(renderer);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {

                // Show menu on escape
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }

                const SDL_Keysym &keysym = e.key.keysym;


                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                               "Keydown code: %u", keysym.scancode);

            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
