#include "SDL3/SDL_render.h"
#include <entt/entt.hpp>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

struct GravityData {
    float mass;
};

struct Image {
    SDL_Texture* texture = nullptr;
};

