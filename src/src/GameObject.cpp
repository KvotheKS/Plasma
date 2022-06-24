#pragma once

#include "../include/Game.hpp"
#include "../include/GameObject.hpp"
#include <memory>

GameObject::GameObject()
{
    this->isDead = false;
    this->started = false;
}

GameObject::~GameObject()
{
    this->components.clear();
}

void GameObject::Start()
{
    for(std::vector<std::shared_ptr<Component>>::iterator it = this->components.begin();
        it != this->components.end(); ++it)
    {
        (*it)->Start();
    }
    this->started = true;
}
    
void GameObject::Update(float dt)
{
    
    for(std::vector<std::shared_ptr<Component>>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
        (*it)->Update(dt);
    
}

void GameObject::Render()
{
    Game& gref = Game::GetInstance();
    
    for(std::vector<std::shared_ptr<Component>>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Render();
    }
}

bool GameObject::IsDead() { return this->isDead; }


void GameObject::RequestDelete() { this->isDead = true;}

void GameObject::AddComponent(Component* cpt) 
{ 
    if(this->started)
        cpt->Start();
    this->components.push_back(std::shared_ptr<Component>(cpt)); 
}

void GameObject::RemoveComponent(Component* cpt)
{
    for(std::vector<std::shared_ptr<Component>>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if(cpt == (*it).get())
        {
            this->components.erase(it);
            break;
        }
    }
}

void GameObject::AddComponents(std::initializer_list<Component*> cpts)
{
    for(std::initializer_list<Component*>::iterator it = cpts.begin();
                it != cpts.end(); ++it)
    {
        if(this->started)
            (*it)->Start();
        this->components.push_back(std::shared_ptr<Component>(*it));
    }
}

Component* GameObject::GetComponent(const std::string& type)
{
    for(std::vector<std::shared_ptr<Component>>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if((*it)->Is(type))
            return (*it).get();
    }

    return nullptr;
}