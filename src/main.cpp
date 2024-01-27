#include <SDL.h>
#include "const.h"
#include "map.h"
#include "player.h"
#include "engine.h"
#include "minimap.h"

const int screenWidth = SCREEN_WIDTH;
const int screenHeight = SCREEN_HEIGHT;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_GL_DOUBLEBUFFER);

    Player player(PLAYER_X, PLAYER_Y, PLAYER_YAW);
    Engine engine(player, screenWidth, screenHeight);
    Minimap minimap(player, screenWidth, screenHeight);

    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame

    bool quit = false;
    SDL_Event e;

    while (!quit) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        engine.render(renderer);
        minimap.render(renderer);

        SDL_RenderPresent(renderer);

        //timing for input and FPS counter
        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
//        print(1.0 / frameTime); //FPS counter
//        redraw();
//        cls();


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

                player.onKeyDown(keysym);
                minimap.onKeyDown(keysym);
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
