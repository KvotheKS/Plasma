#pragma once

#include "./decl.hpp"

class Camera
{
private:
    static GameObject* focus;

public:
    static Vec2 pos, speed;

public:
    static void Follow(GameObject* newFocus);
    static void Unfollow();

public:
    static void Update(float dt);
};