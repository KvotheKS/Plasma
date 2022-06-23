#pragma once 

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include "./decl.hpp"
#include "./Component.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

class Sprite : public Component
{
private:
    SDL_Texture* texture;
    int width, height;
    SDL_Rect clipRect;

public:
    Sprite();

    Sprite(const std::string& file);

    Sprite(GameObject& associated);

    Sprite(GameObject& associated, const std::string& file);

    ~Sprite();

public:
    void Open(const std::string& file, int x, int y, int w=-1, int h=-1);

    void Open(const std::string& file);

    void SetClip(int x, int y, int w, int h);

public:
    void Update(float dt) {};
    void Render();
    void Render(float x, float y);
public:
    int GetWidth();
    int GetHeight();
    bool IsOpen();

public:
    bool Is(const std::string& type);
};