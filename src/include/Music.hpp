#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include <string>

class Music
{
private:
    Mix_Music* music;
public:
    Music();
    Music(const std::string& file);
    ~Music();
public:
    void Play(int times = -1);
    void Stop(int msToStop=1500);
    void Open(const std::string& file);
public:
    bool IsOpen();

};