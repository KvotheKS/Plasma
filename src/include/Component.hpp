#pragma once

#include "./decl.hpp"
#include "./GameObject.hpp"
#include <string>

class Component
{
protected:
    GameObject& associated;

public:
    Component();
    Component(GameObject& associated);
    virtual ~Component();

public:
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

public:
    virtual bool Is(const std::string& type) = 0;
};
