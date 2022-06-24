#pragma once

#include "../include/CameraFollower.hpp"
#include "../include/GameObject.hpp"
#include "../include/Component.hpp"
#include "../include/Camera.hpp"
#include "../include/Game.hpp"
#include <string>

CameraFollower::CameraFollower(GameObject& go) : Component(go) {}

void CameraFollower::Update(float dt) 
{
    const Vec2 cPos = Camera::pos;
    this->associated.box.x = cPos.x;
    this->associated.box.y = cPos.y;
}

void CameraFollower::Render() {}

bool CameraFollower::Is(const std::string& type) 
{ return type == "CameraFollower"; }