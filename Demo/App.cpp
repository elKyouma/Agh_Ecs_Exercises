#include "App.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "SDL3_image/SDL_image.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include "Components.hpp"

App::App()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	if (!SDL_CreateWindowAndRenderer("Hello SDL", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError()); 

    Initialise();
}

App::~App()
{
    Clean();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void App::AddParticle()
{
    const auto entity = ecs.create();
    ecs.emplace<Position>(entity, 40 * (2.f * rand() / RAND_MAX - 1.f), 0.0f);
    
    auto& vel = ecs.emplace<Velocity>(entity);
    vel.dx = (2.f * rand() / RAND_MAX - 1.f) / 2.f;
    vel.dy = 1.f;
    
    float speed = 70 + 25 * (2.f * rand() / RAND_MAX - 1.f);

    float max = sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
    vel.dx *= speed / max;
    vel.dy *= speed / max;


    ecs.emplace<GravityData>(entity, 60 + 5 * (2.f * rand() / RAND_MAX));
    ecs.emplace<Image>(entity, texture);
}

void App::Initialise()
{
    //Get the path to folder with art
    std::string path = ART_PATH;
	path += "gandalf.jpg";
    
    //Load graphic to texture used by particles
    texture = IMG_LoadTexture(renderer, path.c_str());
    timer = reloadTime;
}

void App::Clean()
{ 
	SDL_DestroyTexture(texture);
}

bool App::ProcessEvents()
{
    bool isQuiting = false;

    //Get new window
    SDL_Event event;
    SDL_PollEvent(&event);

    //If we gets quit window message return value is true
    if (event.type == SDL_EVENT_QUIT)
		isQuiting = true;

    return isQuiting;

}

void App::Update()
{
    {
        //Calculate new deltaTime
        Uint64 thisFrameStart = SDL_GetPerformanceCounter();
        deltaTime =  (float)((thisFrameStart - prevFrameStart) / (double)SDL_GetPerformanceFrequency() );
        prevFrameStart = thisFrameStart;
    }

    //Add new particles as long as we dont have max amount of them
    static int particleCount;
    timer -= deltaTime;
    if(timer < 0)
    {
        timer = reloadTime;
        if(particleCount < numberOfParticles)
        {    
            for(int i = 0; i < particlesPerBurst; i++)
                AddParticle();
            particleCount += particlesPerBurst;
        }
    }

    //Change all the positions by velocity
    auto motionView = ecs.view<Position, const Velocity>();
    motionView.each([&](Position &pos, const Velocity &vel) 
              {
                pos.x += vel.dx * deltaTime;
                pos.y += vel.dy * deltaTime;
              });
    
    //Add to velocity in y-axis speed. It is time-dependent and we want to
    //attract more the heavier objects
    auto gravityView = ecs.view<Velocity, const GravityData>();
    gravityView.each([&](Velocity &vel, const GravityData& grav) 
              {
                vel.dy += grav.mass * (-9.81f) * deltaTime;
              });
   
}

void App::Render()
{
    //Clear screen from previous frame
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	
    //Render images at specified position
    auto view = ecs.view<const Position, const Image>();
    view.each([&](const Position& pos, const Image& img) 
        {
            float imgSize = 50.f;

            //We subdivide half imgSize to make the center of image be at position of pos
            SDL_FRect particleRect { SCREEN_WIDTH/2.f + (pos.x - imgSize/2), 
                                    SCREEN_HEIGHT/2.f - (pos.y + imgSize/2), 
                                    imgSize, 
                                    imgSize};
            
            SDL_RenderTexture(renderer, img.texture, NULL, &particleRect);
        });

    //Present frame (send frame to gpu to render it)
    SDL_RenderPresent(renderer);
}
