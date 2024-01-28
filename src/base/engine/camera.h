//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#ifndef RAYCASTING_CAMERA_H
#define RAYCASTING_CAMERA_H

#define FOV 66
#define FOV_RAD (FOV * M_PI / 180)

#define MOVE_SPEED 5.0
#define ROTATE_SPEED 3.0

#include <SDL.h>

const double PLANE_X_RADIUS = tan(FOV_RAD / 2);

inline static double toRadians(int degrees) { return degrees * M_PI / 180; }

class Camera {
public:
    explicit Camera(double posX, double posY, double yawRad);

    [[maybe_unused]] explicit Camera(double posX, double posY, int yawDeg) : Camera(posX, posY, toRadians(yawDeg)) {}

    void onKeyDown(const SDL_Keysym &key);

    [[nodiscard]] double getRayDirX(const double cameraX) const { return dirX + planeX * cameraX; };

    [[nodiscard]] double getRayDirY(const double cameraX) const { return dirY + planeY * cameraX; };

    //x and y start position
    double posX, posY;
    //initial direction vector
    double dirX, dirY;
    //the 2d raycaster version of camera plane
    double planeX, planeY;
};


#endif //RAYCASTING_CAMERA_H
