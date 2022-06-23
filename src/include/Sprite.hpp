#pragma once 

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include <iostream>
#include <string>
#include <stdexcept>

class Sprite
{
private:
    SDL_Texture* texture;
    int width, height;
    SDL_Rect clipRect;

public:
    Sprite();

    Sprite(const std::string& file);

    ~Sprite();

public:
    void Open(const std::string& file, int x, int y, int w=-1, int h=-1);

    void Open(const std::string& file);

    void SetClip(int x, int y, int w, int h);

public:
    void Render(int x, int y);

public:
    int GetWidth();
    int GetHeight();
    bool IsOpen();
};
