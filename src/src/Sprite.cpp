#pragma once

#include "../include/Sprite.hpp"
#include "../include/Game.hpp"
#include "../include/Resources.hpp"

///////////////////////////////////////////////// Game Sprites ///////////////////////////////////////////////

Sprite::Sprite() : Component()
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->clipRect = {0, 0, 0, 0};
}

Sprite::Sprite(GameObject& associated, const std::string& file)
 : Component(associated)
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    
    Open(file);
}

Sprite::~Sprite()
{
    this->texture = nullptr;
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
    this->texture = Resources::GetImage(file);

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

void Sprite::Render()
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    const Rect& wrp = this->associated.box;
    SDL_Rect dst = {
        (int)wrp.x,
        (int)wrp.y,
        (int)wrp.w,
        (int)wrp.h
    };
    
    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

void Sprite::Render(float x, float y)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    
    SDL_Rect dst = {
        (int)x,
        (int)y,
        this->clipRect.w, 
        this->clipRect.h
    };

    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}


int Sprite::GetWidth(){ return this->width; }
int Sprite::GetHeight(){ return this->height; }
bool Sprite::IsOpen(){ return this->texture != nullptr; }
bool Sprite::Is(const std::string& type) { return type == "Sprite"; }