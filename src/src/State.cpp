#pragma once

#define _USE_MATH_DEFINES
#include "../include/State.hpp"
#include "../include/Rect.hpp"
#include "../include/Sprite.hpp"
#include "../include/Sound.hpp"
#include "../include/Music.hpp"
#include "../include/Face.hpp"
#include "../include/TileSet.hpp"
#include "../include/TileMap.hpp"
#include "../include/GameObject.hpp"
#include "../include/CameraFollower.hpp"
#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"
#include "../include/Alien.hpp"
#include "../include/Error.hpp"
#include <memory>

//////////////////////////////////////////////// Game State Functions /////////////////////////////////////////////////

State::State() 
{
    this->started = false;
    this->quitRequested = false;
}

State::~State()
{
    this->objectArray.clear();
}

void State::LoadAssets()
{
    Game& inst = Game::GetInstance();
    Sprite* spr = new Sprite(this->bg, "./resources/img/ocean.jpg");
    Music* msc = new Music(this->bg, "./resources/audio/stageState.ogg");

    this->bg.box = { 0, 0, (float)inst.GetWidth(), (float)inst.GetHeight() };
    msc->Play();
    this->bg.AddComponents({
        spr,
        msc,
        new CameraFollower(this->bg)
    });
    

    GameObject* go = new GameObject();
    
    TileMap* map = new TileMap(
        *go , 
        "./resources/map/tileMap.txt", 
        new TileSet(64, 64, "./resources/img/tileSet.png")
    );

    go->box = { 0, 0, (float)map->GetPWidth(), (float)map->GetPHeight() };
    
    go->AddComponent(map);
    AddObject(go);
    this->objectArray.push_back(std::shared_ptr<GameObject>(go));

    go = new GameObject();
    Alien* al = new Alien(*go, 5);
    go->AddComponent(al);
    AddObject(go);
}

void State::Start()
{
    this->started = true;
    LoadAssets();
}

void State::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();
    
    if(SDL_QuitRequested() || controller.QuitRequested())
    {
        this->quitRequested = true;
        return;
    }

    Camera::Update(dt);
    
    int vSize = this->objectArray.size();
    
    for(int i = 0; i < vSize; i++)
    {
        if(this->objectArray[i]->IsDead())
        {
            this->objectArray.erase(this->objectArray.begin() + i);    
            vSize--;
            i--;
        }
    }

    this->bg.Update(dt);

    vSize = this->objectArray.size();

    for(int i = 0; i < vSize; i++)
        this->objectArray[i]->Update(dt);
}

void State::Render()
{
    if(this->quitRequested)
        return;

    this->bg.Render();
    for(std::vector<std::shared_ptr<GameObject>>::iterator it = this->objectArray.begin();
            it != this->objectArray.end(); ++it)
        (*it)->Render();
}

std::weak_ptr<GameObject> State::AddObject(GameObject* go)
{
    std::shared_ptr<GameObject> ptr(go);
    this->objectArray.push_back(ptr);
    
    if(this->started)
        ptr->Start();
    
    return ptr;
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go)
{
    for(std::vector<std::shared_ptr<GameObject>>::iterator it = this->objectArray.begin();
        it != this->objectArray.end(); it++)
    {
        if((*it).get() == go)
            return *it;
    }
    return std::weak_ptr<GameObject>();
}

bool State::QuitRequested() { return this->quitRequested; }