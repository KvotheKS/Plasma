#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "./Sprite.hpp"
#include "./Music.hpp"


class State
{
private:
    Sprite bg; 
    Music music;
    bool quitRequested;
public:
    State();

    bool QuitRequested();
    
    void LoadAssets();
    
    void Update(float dt);

    void Render();
};
