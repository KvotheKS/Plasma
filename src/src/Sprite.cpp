#pragma once

#include "../include/Sprite.hpp"
#include "../include/Game.hpp"

///////////////////////////////////////////////// Game Sprites ///////////////////////////////////////////////

Sprite::Sprite()
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->clipRect = {0, 0, 0, 0};
}

Sprite::Sprite(const std::string& file)
{
    Sprite();
    Open(file);
}

Sprite::~Sprite()
{
    if(this->texture)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Sprite::Open(const std::string& file, int x, int y, int w, int h)
{
    Open(file);
    
    w = w == -1 ? this->width : w;
    h = h == -1 ? this->height : h;

    SetClip(x,y,w,h);
}

void Sprite::Open(const std::string& file)
{
    const auto renderer = Game::GetInstance().GetRenderer();

    if(this->texture)
        SDL_DestroyTexture(this->texture);
    this->texture = IMG_LoadTexture(renderer ,file.c_str());

    if(this->texture == nullptr)
        Game::ThrowException();
    
    SDL_QueryTexture(this->texture, nullptr, nullptr, &this->width, &this->height);
    SetClip(0,0,this->width, this->height);
}

void Sprite::SetClip(int x, int y, int w, int h)
{
    this->clipRect = {
        x, y, w, h
    };
}

void Sprite::Render(int x, int y)
{
    const auto renderer = Game::GetInstance().GetRenderer();
    SDL_Rect dst = {
        x, y, this->clipRect.w, this->clipRect.h
    };

    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

int Sprite::GetWidth(){ return this->width; }
int Sprite::GetHeight(){ return this->height; }
bool Sprite::IsOpen(){ return this->texture != nullptr; }