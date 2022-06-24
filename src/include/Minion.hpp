#pragma once

#include "./decl.hpp"
#include "./Component.hpp"
#include "./GameObject.hpp"
#include <memory>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>


class Minion : public Component
{
private:
    std::weak_ptr<GameObject> alienCenter;
    float arc;
    const float speed;
    float bSpeed;
    int damage; 

public:
    Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg = 0.0f);

public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);

public:
    void Shoot(const Vec2 target);
};