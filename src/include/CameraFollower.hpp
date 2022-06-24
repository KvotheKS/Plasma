#pragma once


#include "./decl.hpp"
#include "./Component.hpp"
#include "./GameObject.hpp"
#include <string>

class CameraFollower : public Component
{
public:
    CameraFollower(GameObject& go);

public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type);
};