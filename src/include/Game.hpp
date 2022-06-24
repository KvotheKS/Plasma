#pragma once
    
#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "./decl.hpp"
#include "./State.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
 
class Game
{
private:
    static Game* instance;
    static bool allocated;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    State* state = nullptr;

private:
    int WindowWidth, WindowHeight;

private:
    int fps;
    float period;
    int frameStart;
    float dt;

public:
    static void ThrowException()
    {
        throw std::runtime_error(SDL_GetError());
        abort();
    }

private:
    Game(const char* Title = "Titulo", int width = 600, int height = 480);

    void InitConfigs(int width, int height);

public:
    ~Game();

public:
    SDL_Renderer* GetRenderer();
    
    State& GetState();
    
    static Game& GetInstance(const char* Title = "Titulo", int width = 600, int height = 480);

public:
    void CalculateDeltaTime();
    float GetDeltaTime();

public:
    int GetWidth();
    int GetHeight();

public:
    void Run();
};