#pragma once

#include "../include/Component.hpp"
#include "../include/Sprite.hpp"
#include "../include/Face.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"

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
    Sound* sptr = ((Sound*)this->associated.GetComponent("Sound"));
    if(sptr)
        sptr->Play();
    this->associated.RequestDelete();
}

void Face::Update(float dt) 
{
    InputManager& controller = InputManager::GetInstance();
    const Vec2 cPos = Camera::pos;

    if(controller.MousePress(LEFT_MOUSE_BUTTON) && 
        this->associated.box.in(Vec2(controller.GetMouseX() + cPos.x, controller.GetMouseY() + cPos.y)))
    {
        Damage(30);
    }
}

void Face::Render() {}


bool Face::Is(const std::string& type) { return type == "Face";}