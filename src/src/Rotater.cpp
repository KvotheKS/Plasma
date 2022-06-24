#pragma once

#include "../include/Component.hpp"
#include "../include/GameObject.hpp"
#include "../include/Rotater.hpp"
#include "../include/Sprite.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

Rotater::Rotater(GameObject& associated, float arc, float speed)
    : Component(associated), arc(arc), speed(speed)
{}

void Rotater::Update(float dt)
{
    this->arc += speed*dt;
    if(this->arc > 2*M_PI)
        this->arc -= 2*M_PI;
    else if(this->arc < -2*M_PI)
        this->arc += 2*M_PI;
        
    Sprite* spr = (Sprite*)this->associated.GetComponent("Sprite");
    spr->angleDeg = this->arc*(180.0/M_PI);
}

void Rotater::Render() {}

bool Rotater::Is(const std::string& type)
{ return type == "Rotater";}