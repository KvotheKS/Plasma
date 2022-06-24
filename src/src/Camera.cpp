#pragma once

#include "../include/Camera.hpp"
#include "../include/InputManager.hpp"
#include "../include/GameObject.hpp"
#include <algorithm>

Vec2 Camera::pos = {0,0}, Camera::speed = {10,10};
GameObject* Camera::focus = nullptr;

void Camera::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();
    if(Camera::focus)
        Camera::pos = Camera::focus->box.center();
    else
    {
        Camera::pos.x += (controller.IsKeyDown(RIGHT_ARROW_KEY) - controller.IsKeyDown(LEFT_ARROW_KEY))*speed.x;
        Camera::pos.y += (controller.IsKeyDown(DOWN_ARROW_KEY) - controller.IsKeyDown(UP_ARROW_KEY))*speed.y;
        
        Camera::pos.x = std::max(Camera::pos.x, 0.0f);
        Camera::pos.y = std::max(Camera::pos.y, 0.0f);
    }
}

void Camera::Follow(GameObject* newFocus) { Camera::focus = newFocus; }
void Camera::Unfollow() { Camera::focus = nullptr; }