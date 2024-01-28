//
// Created by Evgenii Shchepotev on 28.01.2024.
//

#ifndef RAYCASTING_RAYCASTENGINE_H
#define RAYCASTING_RAYCASTENGINE_H

#include "intersection.h"
#include "../renderer/pixelrenderer.h"
#include "camera.h"

class RaycastEngine {
public:
    explicit RaycastEngine(const Camera &camera) : camera(camera) {
    };

    void render(PixelRenderer *renderer);

private:
    TextureMap textureMap;

    const Camera &camera;

    Intersection trace(double cameraX);

    [[maybe_unused]] void drawColor(SDL_Renderer *renderer, int x,
                                    const Intersection &intersection,
                                    int drawStart,
                                    int drawEnd) const;
};


#endif //RAYCASTING_RAYCASTENGINE_H
