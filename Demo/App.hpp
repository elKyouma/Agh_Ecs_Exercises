#pragma once
#include "SDL3/SDL_render.h"
#include <chrono>
#include <SDL3/SDL_render.h>
#include <cstdint>
#include <entt/entt.hpp>

class App
{
public:
    App();
    ~App();

    //Update logic
    void Update();

    //Analyze inputs, do action and return true if 
    // it got quiting app message
    bool ProcessInputs();
    
    //Draw everything to window
    void Render();

private:
    //Called at the end of constructor
    void Initialise();
    //Called before destructor
    void Clean();

    //Adds new particle to our world 
    void AddParticle();

    //Texture that all particles uses
	SDL_Texture* texture;

    //Things SDL uses for renderingand window creation process
	SDL_Window* window;
	SDL_Renderer* renderer;    

private:
    static constexpr uint16_t SCREEN_WIDTH = 1000;
    static constexpr uint16_t SCREEN_HEIGHT = 800;

    static constexpr int numberOfParticles = 300; //Particles max amount
    static constexpr int particlesPerBurst = 10; //Speed of spawning new particles
    static constexpr float reloadTime = 0.1; //Tells how often you should spawn particles
    
    float deltaTime;
    float timer;

    //Just time from previous frame. Why c++ why???
    std::chrono::time_point<std::chrono::high_resolution_clock> prevFrameStart{std::chrono::high_resolution_clock::now()};

    //Entt Ecs
    entt::registry ecs;
};
