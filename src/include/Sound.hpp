#pragma once

#include "./decl.hpp"
#include "./Component.hpp"
#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"

 class Sound : public Component
{
private:
    Mix_Chunk* chunk;
    int channel;

public:
    Sound();
    Sound(GameObject& associated);
    Sound(GameObject& associated, const std::string& file);
    ~Sound();

public:
    void Play(int times = 0);
    void Stop();

public:
    void Open(const std::string& file);
    bool IsOpen();
    bool IsPlaying();
public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);
};
