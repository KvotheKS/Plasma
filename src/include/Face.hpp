#pragma once

#include "./decl.hpp"
#include "./Component.hpp"
#include "./Sound.hpp"
#include <string>

class Face : public Component
{
private:
    int hitpoints;

public:
    Face();
    Face(GameObject& associated);
    ~Face();
public:
    void Damage(int damage);
    void Death();
public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);
};