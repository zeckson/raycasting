//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#include "game.h"

Game::Game(const char* title, Uint16 width, Uint16 height): width(width), height(height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_Renderer *pSdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    renderer = pSdlRenderer;
}

void Game::start() {
    Uint32 time = 0; //time of current frame
    Uint32 oldTime = 0; //time of previous frame

    double frameTime = 0;

    onGameCreate();

    bool quit = false;
    SDL_Event e;
    while (!quit) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        onGameUpdate(frameTime);

        SDL_RenderPresent(renderer);

        //timing for input and FPS counter
        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
//        print(1.0 / frameTime); //FPS counter
//        redraw();
//        cls();


        quit = input(e);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::input(SDL_Event &e) {
    bool quit;
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

            onKeyDown(keysym);
        }
    }
    return quit;
}
