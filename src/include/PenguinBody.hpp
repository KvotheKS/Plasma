#pragma once

#include "./Component.hpp"
#include "./GameObject.hpp"
#include <string>
#include <memory>

class PenguinBody : public Component
{
private:
    std::weak_ptr<GameObject> pCannon;
    Vec2 speed;
    Vec2 spdLimit;
    float linearSpeed, angle;
    float rotSpeed;
    int hp;

public:
    static PenguinBody* player;

public:
    PenguinBody(GameObject& associated);
    ~PenguinBody();

public:
    void Start();
    void Update(float dt);
    void Render();

public:
    void Accelerate(int mv, float dt);
    void Rotate(int mv, float dt);

public:
    bool Is(const std::string& type); 
};