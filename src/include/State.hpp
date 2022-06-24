#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include <memory>
#include <vector>
#include <iostream>
#include "SDL_include.h"
#include "./decl.hpp"
#include "./GameObject.hpp"
#include "./Game.hpp"

class State
{
private:
    GameObject bg;
    std::vector<std::shared_ptr<GameObject>> objectArray;
    bool quitRequested, started;
public:
    State();
    ~State();
public:
        
    bool QuitRequested();
public:
    void Start();

    void LoadAssets();
    
public:
    void Update(float dt);

    void Render();

public:
    std::weak_ptr<GameObject> AddObject(GameObject* go);
    std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);
};
