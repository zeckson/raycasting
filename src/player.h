//
// Created by Evgenii Shchepotev on 23.01.2024.
//

#ifndef RAYCASTING_PLAYER_H
#define RAYCASTING_PLAYER_H

#define MOVE_SPEED 5.0
#define ROTATE_SPEED 3.0

#include <SDL_events.h>

struct Intersection {
    Intersection(int x, int y, double distance) : x(x), y(y), distance(distance) {}

    int x, y;
    double distance;
};

class Player {
private:
    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

public:
    int getMapX() { return int(posX); }
    int getMapY() { return int(posY); }

    Intersection trace(double ray);

    void onKeyDown(const SDL_Keysym &key);
};


#endif //RAYCASTING_PLAYER_H
