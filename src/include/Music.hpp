#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "decl.hpp"
#include "./Component.hpp"
#include <string>


class Music : public Component
{
private:
    Mix_Music* music;
public:
    Music() : Component() { this->music = nullptr; }
    Music(GameObject& associated, const std::string& file);
    ~Music();
public:
    void Play(int times = -1);
    void Stop(int msToStop=1500);
    void Open(const std::string& file);
public:
    
    void Update(float dt);
    void Render();
public:
    bool IsOpen();
public:
    bool Is(const std::string& type);
};