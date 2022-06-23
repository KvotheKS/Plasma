#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include <memory>
#include <vector>
#include <iostream>
#include "SDL_include.h"
#include "./decl.hpp"
#include "./GameObject.hpp"
#include "./Game.hpp"

class State
{
private:
    GameObject bg;
    bool quitRequested;
    std::vector<std::unique_ptr<GameObject>> objectArray;
public:
    State();
    ~State();
public:
        
    bool QuitRequested();
    
    void LoadAssets();
    
    void Update(float dt);

    void Render();

public:
    void Input();
    void AddObject(int mouseX, int mouseY);
};
