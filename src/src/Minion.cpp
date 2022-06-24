#pragma once

#include "../include/Minion.hpp"
#include "../include/GameObject.hpp"
#include "../include/Component.hpp"
#include "../include/Sprite.hpp"
#include "../include/Vec2.hpp"
#include "../include/Bullet.hpp"
#include "../include/Game.hpp"
#include "../include/State.hpp"
#include "../include/Component.hpp"
#include "../include/Rotater.hpp"
#include <cstdlib>
#include <memory>

Minion::Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg)
    : Component(associated), speed(M_PI/2.0f), bSpeed(400.0f),
      damage(20)
{
    this->alienCenter = alienCenter;
    this->arc = arcOffsetDeg;

    Sprite* spr = new Sprite(associated, "./resources/img/minion.png");

    const float rx = 1.0f + ((float)rand())/(((float)RAND_MAX)*2.0f);
    const float ry = 1.0f + ((float)rand())/(((float)RAND_MAX)*2.0f);
    
    spr->SetScale(rx, ry);

    associated.AddComponent(spr);
    
    associated.box.w = spr->GetWidth();
    associated.box.h = spr->GetHeight();
    associated.AddComponent(new Rotater(associated, this->arc, this->speed));
}

void Minion::Update(float dt)
{
    std::shared_ptr<GameObject> ptr = this->alienCenter.lock();
    this->arc += (this->speed*dt);
    if(this->arc > M_PI*2)
        this->arc -= M_PI*2;    
    if(ptr == std::shared_ptr<GameObject>())
    {
        this->associated.RequestDelete();
        return;
    }
    Vec2 offset = Vec2(ptr->box.w/2 + ptr->box.h/2, 0);
    offset.Rotate(this->arc);
    offset += ptr->box.center();
    
    this->associated.box.setCenter(offset);
}

void Minion::Render()
{}

void Minion::Shoot(Vec2 target)
{
    State& st = Game::GetInstance().GetState();
    GameObject* go = new GameObject;
    float angl = (target-this->associated.box.center()).theta();
    Vec2 tCen = this->associated.box.center();
    
    go->box = Rect(tCen.x, tCen.y);
    
    Bullet* ptr = new Bullet(*go, angl,this->bSpeed, this->damage, 1200,
        "./resources/img/minionbullet1.png");
    
    go->AddComponent(ptr);
    
    st.AddObject(go);
}

bool Minion::Is(const std::string& type) { return type == "Minion"; }