#pragma once

#include "./decl.hpp"
#include "./Component.hpp"

class Bullet : public Component
{
private:
    Vec2 speed;
    float distanceLeft;
    int damage;

public:
    Bullet(GameObject& associated, float angle, float speed, 
            int damage, float maxDistance, Sprite* sprite);
    
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);

public:
    int GetDamage();
};