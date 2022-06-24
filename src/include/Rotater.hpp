#pragma once

#include "./Component.hpp"
#include "./GameObject.hpp"
#include <string>

class Rotater : public Component
{
private:
    float arc, speed;
public:
    Rotater(GameObject& associated, float arc, float speed);

public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);
};