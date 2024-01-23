#include <cmath>
#include <SDL.h>
#include "map.h"
#include "player.h"

const int screenWidth = 800;
const int screenHeight = 600;

struct ColorRGB {
    int r, g, b, a;

    ColorRGB(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
    ColorRGB(int r, int g, int b) : ColorRGB(r, g, b, 255) {}
};

static const ColorRGB GREEN(0, 255, 0);
static const ColorRGB RED(255, 0, 0);
static const ColorRGB BLUE(0, 0, 255);
static const ColorRGB WHITE(255, 255, 255);
static const ColorRGB YELLOW(255, 255, 0);

static const ColorRGB colorMap[] = {
        WHITE, YELLOW, RED, GREEN, BLUE
};

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player;

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

            double perpWallDist = player.getRayDistance(cameraX);

            //Calculate height of line to draw on screen
            int lineHeight = (int) (h / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h) drawEnd = h - 1;

            int mapX = player.getMapX();
            int mapY = player.getMapY();

            //choose wall color
            ColorRGB color = colorMap[worldMap[mapX][mapY]];

            //give x and y sides different brightness
//            if (side == 1) { color = color / 2; }


            // Draw the column
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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
