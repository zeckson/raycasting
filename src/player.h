//
// Created by Evgenii Shchepotev on 23.01.2024.
//

#ifndef RAYCASTING_PLAYER_H
#define RAYCASTING_PLAYER_H


class Player {
private:
    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

public:
    int getMapX() { return int(posX); }
    int getMapY() { return int(posY); }

    double getRayDistance(double x);
};


#endif //RAYCASTING_PLAYER_H
