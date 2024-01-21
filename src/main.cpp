#include <iostream>
#include <cmath>
#include <vector>
#include <SDL.h>

const int screenWidth = 800;
const int screenHeight = 600;

struct Vector2 {
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
};

struct Ray {
    Vector2 origin;
    float angle;

    Ray(const Vector2& _origin, float _angle) : origin(_origin), angle(_angle) {}
};

struct Wall {
    Vector2 start, end;

    Wall(const Vector2& _start, const Vector2& _end) : start(_start), end(_end) {}
};

struct Player {
    Vector2 position;
    float angle;

    Player(const Vector2& _position, float _angle) : position(_position), angle(_angle) {}
};

float degreesToRadians(float degrees) {
    return degrees * M_PI / 180.0f;
}

float normalizeAngle(float angle) {
    angle = fmod(angle, 360.0f);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool castRay(const Ray& ray, const Wall& wall, Vector2& intersection) {
    float x1 = wall.start.x;
    float y1 = wall.start.y;
    float x2 = wall.end.x;
    float y2 = wall.end.y;

    float x3 = ray.origin.x;
    float y3 = ray.origin.y;
    float x4 = ray.origin.x + cos(degreesToRadians(ray.angle));
    float y4 = ray.origin.y + sin(degreesToRadians(ray.angle));

    float denominator = (x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2);

    if (denominator == 0) {
        return false;
    }

    float t = ((x1 - x3) * (y3 - y4) - (x3 - x4) * (y1 - y3)) / denominator;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

    if (t > 0 && t < 1 && u > 0) {
        intersection.x = x1 + t * (x2 - x1);
        intersection.y = y1 + t * (y2 - y1);
        return true;
    }

    return false;
}

void renderScene(SDL_Renderer* renderer, const Player& player, const std::vector<Wall>& walls) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < screenWidth; ++x) {
        float rayAngle = player.angle - 30.0f + x * (60.0f / screenWidth);
        rayAngle = normalizeAngle(rayAngle);

        Ray ray(player.position, rayAngle);

        float closestDistance = std::numeric_limits<float>::infinity();
        Vector2 closestIntersection(0.0f, 0.0f);  // Initialize with default values

        for (const Wall& wall : walls) {
            Vector2 intersection;
            if (castRay(ray, wall, intersection)) {
                float dist = distance(player.position.x, player.position.y, intersection.x, intersection.y);
                if (dist < closestDistance) {
                    closestDistance = dist;
                    closestIntersection = intersection;
                }
            }
        }

        int lineHeight = static_cast<int>(screenHeight / closestDistance);

        // Draw the column
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderDrawLine(renderer, x, (screenHeight - lineHeight) / 2, x, (screenHeight + lineHeight) / 2);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Define the player and walls
    Player player(Vector2(75, 20), 0.0f);
    std::vector<Wall> walls = {
            Wall(Vector2(30, 10), Vector2(30, 30)),
            Wall(Vector2(30, 30), Vector2(50, 30)),
            Wall(Vector2(50, 30), Vector2(50, 10)),
            Wall(Vector2(50, 10), Vector2(30, 10)),
    };

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        renderScene(renderer, player, walls);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
