#pragma once

#include "../include/PenguinBody.hpp"
#include "../include/InputManager.hpp"
#include <string>
#include <memory>
#include <cmath>

PenguinBody* PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject& associated)
{
    PenguinBody::player = this;
    this->speed = {0.0f, 0.0f};
    this->spdLimit = {100.0f, 100.0f};
    this->angle = 0.0f;
    this->rotSpeed = M_PI/1.5f;
    this->linearSpeed = 4.0f;
    this->hp = 30;
    
    Sprite* spr = new Sprite(associated, "./resources/img/penguin.png");
    associated.box = Rect(500,200,spr->GetWidth(),spr->GetHeight());
    associated.AddComponents({
        this,
        spr
    });
}

PenguinBody::~PenguinBody()
{
    PenguinBody::player = nullptr;
}

void PenguinBody::Start()
{}

void PenguinBody::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();    
    this->Rotate( controller.IsKeyDown(D_KEY) - controller.IsKeyDown(A_KEY), dt);
    this->Accelerate(controller.IsKeyDown(S_KEY) - controller.IsKeyDown(W_KEY), dt);
}

void PenguinBody::Rotate(int mv, float dt)
{
    if(mv != 0)
    {

        this->angle += ((float)mv)*this->rotSpeed*dt;
        if(this->angle < -2*M_PI)
            this->angle += 2*M_PI;
        else if(this->angle > 2*M_PI)
            this->angle -= 2*M_PI;
    }
}

void PenguinBody::Accelerate(int mv, float dt)
{
    if(mv != 0)
    {
        
    }
}

