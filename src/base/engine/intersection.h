//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#ifndef RAYCASTING_INTERSECTION_H
#define RAYCASTING_INTERSECTION_H

enum class CellSide {
    EAST = 0,
    WEST,
    NORTH,
    SOUTH,
    UNKNOWN = 99
};

struct Intersection {
    Intersection(int x, int y, double distance, CellSide side) : x(x), y(y), distance(distance), side(side) {}

    int x, y;
    double distance;
    CellSide side;
};
#endif //RAYCASTING_INTERSECTION_H
