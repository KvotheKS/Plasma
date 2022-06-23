#pragma once

#include "../include/Component.hpp"
#include "../include/Sprite.hpp"
#include "../include/Face.hpp"
#include "../include/Game.hpp"

#include <iostream>

Face::Face() {}
Face::Face(GameObject& associated) : Component(associated) 
{ this->hitpoints = 30; }
Face::~Face() {}

void Face::Damage(int damage)
{
    this->hitpoints -= damage;
    
    if(this->hitpoints <= 0)
        Death();    
}

void Face::Death()
{
    
    Sprite* spr = ((Sprite*)this->associated.GetComponent("Sprite"));
    if(spr)
        this->associated.RemoveComponent(spr);

    Sound* sptr = ((Sound*)this->associated.GetComponent("Sound"));
    if(spr && sptr)
        sptr->Play();
    if(!sptr->IsPlaying())
        this->associated.RequestDelete();
    
}

void Face::Update(float dt) {}

void Face::Render() {}


bool Face::Is(const std::string& type) { return type == "Face";}