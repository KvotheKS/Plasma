#pragma once

#include "../include/GameObject.hpp"

GameObject::GameObject()
{
    this->isDead = false;
}

GameObject::~GameObject()
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        delete (*it);
    }
    this->components.clear();
}
    
void GameObject::Update(float dt)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Update(dt);
    }
}

void GameObject::Render()
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Render();
    }
}

bool GameObject::IsDead() { return this->isDead; }


void GameObject::RequestDelete() { this->isDead = true;}

void GameObject::AddComponent(Component* cpt) 
{ this->components.push_back(cpt); }

void GameObject::RemoveComponent(Component* cpt)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if(cpt == *it)
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
        this->components.push_back(*it);
}

Component* GameObject::GetComponent(const std::string& type)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if((*it)->Is(type))
            return *it;
    }

    return nullptr;
}