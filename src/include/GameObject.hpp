#pragma once

#include <vector>
#include "./decl.hpp"
#include "./Component.hpp"
#include "./Rect.hpp"
#include <string>
#include <initializer_list>

class GameObject
{
private:
    std::vector<Component*> components;
    bool isDead;

public:
    Rect box;

public:
    GameObject();
    ~GameObject();

public:
    void Update(float dt);
    void Render();

public:
    bool IsDead();

public:
    void RequestDelete();
    
    void AddComponent(Component* cpt);
    
    void AddComponents(std::initializer_list<Component*> cpts);
    void RemoveComponent(Component* cpt);
    Component* GetComponent(const std::string& type);
};