#pragma once

#include "../include/Camera.hpp"
#include "../include/Game.hpp"
#include "../include/GameObject.hpp"
#include "../include/InputManager.hpp"
#include "../include/Alien.hpp"
#include "../include/Component.hpp"
#include "../include/Sprite.hpp"
#include "../include/Vec2.hpp"
#include "../include/Minion.hpp"
#include "../include/Rotater.hpp"
#include <algorithm>
#include <string>

Alien::Alien(GameObject& associated, int nMinions)
    : Component(associated)
{
    this->hp = 30;
    this->speed = {4,4};
    this->minionArray.resize(nMinions);

    Sprite* spr = new Sprite(associated, "./resources/img/alien.png");
    
    Vec2 cb = {512,300};

    this->associated.box.w = std::max(spr->GetWidth(), spr->GetHeight());
    this->associated.box.h = this->associated.box.w;
    this->associated.box.setCenter(cb);
    this->associated.AddComponent(spr);
    this->associated.AddComponent(new Rotater(this->associated, 0.0f, -M_PI/9.0f));
}

Alien::~Alien() 
{ 
    this->associated.RequestDelete();
    this->minionArray.clear(); 
}

void Alien::Start() 
{
    State& st = Game::GetInstance().GetState();
    
    int vSize = this->minionArray.size();
    std::cout << vSize;
    float arc = 0.0f;
    GameObject* go;
    for(int i = 0; i < vSize; i++, arc += (M_PI*2)/((float)vSize))
    {
        go = new GameObject;
        go->AddComponent(new Minion(*go, st.GetObjectPtr(&this->associated), arc));
        st.AddObject(go);
        this->minionArray[i] = st.GetObjectPtr(go);
    }
}

void Alien::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();
    const Vec2 pos = Camera::pos;

    if(controller.MousePress(LEFT_MOUSE_BUTTON))
        this->taskQueue.push(Alien::Action(Alien::Action::ActionType::SHOOT, 
            controller.GetMouseX() + pos.x, controller.GetMouseY() + pos.y));    
        
    
    if(controller.MousePress(RIGHT_MOUSE_BUTTON))
        this->taskQueue.push(Alien::Action(Alien::Action::ActionType::MOVE, 
            controller.GetMouseX() + pos.x, controller.GetMouseY() + pos.y));    
    
    if(this->taskQueue.size())
    {
        Alien::Action it = this->taskQueue.front();
        if(it.type == Alien::Action::ActionType::MOVE)
            Move(it);
        else
            Shoot(it);
    }
    
    if(this->hp<=0)
        this->~Alien();
}

void Alien::Move(Alien::Action& act)
{
    GameObject& asc = this->associated;
    Vec2 fs = (act.pos - asc.box.center());
    Vec2 nm = fs.norm() * this->speed;
    
    if(abs(fs.x) <= nm.x)
    {
        asc.box.x += fs.x;
        asc.box.y += fs.y;
        this->taskQueue.pop();
        return;
    }
    
    asc.box.x += (fs.x >= 0.0f)*nm.x + (fs.x <= 0.0f)*nm.x;
    asc.box.y += (fs.y >= 0.0f)*nm.y + (fs.y <= 0.0f)*nm.y;
}

void Alien::Shoot(Alien::Action& act)
{
    std::shared_ptr<GameObject> best;
    int vSize = this->minionArray.size();
    for(int i = 0; i < vSize; i++)
    {
        std::shared_ptr<GameObject> lcl = this->minionArray[i].lock();
        if(!best)
        {
            best = this->minionArray[i].lock();
            continue;
        }

        float bestDist = best->box.center().dist(act.pos);
        float lclDist = lcl->box.center().dist(act.pos);
        
        if(bestDist>lclDist)
            best = this->minionArray[i].lock();
        
    }
    if(best)
        ((Minion*)best->GetComponent("Minion"))->Shoot(act.pos);
    
    this->taskQueue.pop();
}

bool Alien::Is(const std::string& type) { return "Alien"; }

Alien::Action::Action(Alien::Action::ActionType type, float x, float y)
   	: type(type), pos(x,y)
{}