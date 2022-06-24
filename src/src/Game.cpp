#pragma once

#include "../include/Game.hpp"
#include "../include/Resources.hpp"
#include "../include/InputManager.hpp"
#include <ctime>
#include <cstdlib>

/////////////////////////////////////////////// Game Base Functions /////////////////////////////////////////////////////////

Game* Game::instance = nullptr;
bool Game::allocated = false;

Game::Game(const char* Title, int width, int height)
{
    if(!Game::instance)
        Game::instance = this;

    // Init SDL2
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        Game::ThrowException();
    }
    // Init SDL_image
    const int imgFlags = IMG_INIT_JPG  |  IMG_INIT_PNG  |  IMG_INIT_TIF; 
    
    if(IMG_Init(imgFlags) == 0)
    {
        Game::ThrowException();
    }
    // Init SDL_mixer
    const int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
    
    if(Mix_Init(mixFlags) == 0)
    {
        Game::ThrowException();
    }
    
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 1024)!=0)
    {
        Game::ThrowException();
    }

    Mix_AllocateChannels(32);

    this->window = SDL_CreateWindow(
        Title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_SHOWN
    );

    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    InitConfigs(width, height);
}

void Game::InitConfigs(int width, int height)
{
    this->fps = 60;
    this->period = 1000.0f/((float)fps);
    this->frameStart = SDL_GetTicks();
    this->dt = 0.0f;

    this->WindowHeight = height;
    this->WindowWidth = width;

    this->state = new State();
    
    srand(time(NULL));
}

Game::~Game()
{
    if(Game::instance)
    {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        
        IMG_Quit();

        Mix_CloseAudio();
        Mix_Quit();
        
        if(Game::allocated)
        {
            Game* hp = Game::instance;
            Game::allocated = false;
            Game::instance = nullptr;
            delete hp;
        }
    }
}

Game& Game::GetInstance(const char* Title, int width, int height)
{  
    if(!Game::instance)
    {
        std::cout << "ok";
        Game::allocated = true;
        Game::instance = new Game(Title, width, height);
    }
    
    return *Game::instance;
}

void Game::Run()
{
    InputManager& controller = InputManager::GetInstance();
    this->state->Start();
    while(!this->state->QuitRequested())
    {
        CalculateDeltaTime();

        controller.Update();

        this->state->Update(dt);
        
        this->state->Render();
        
        SDL_RenderPresent(this->renderer);
        
        SDL_Delay(this->period);
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}

void Game::CalculateDeltaTime()
{
    const int currT = SDL_GetTicks();
    dt = (currT - frameStart)/1000.0f;
    frameStart = currT;
}

float Game::GetDeltaTime() { return this->dt; }

int Game::GetWidth() { return this->WindowWidth; }

int Game::GetHeight() { return this->WindowHeight; }

SDL_Renderer* Game::GetRenderer(){ return this->renderer; }
    
State& Game::GetState(){ return *this->state; }
    