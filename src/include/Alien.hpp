#pragma once

#include "./decl.hpp"
#include "./Component.hpp"
#include "./Vec2.hpp"
#include "./GameObject.hpp"
#include <vector>
#include <queue>
#include <memory>
#include <string>

class Alien : public Component
{
private:
    class Action;
    class Action
    {    
    public:
        enum ActionType
        {
            MOVE, SHOOT
        };

    public:
        ActionType type;
        Vec2 pos;
    public:
        Action(ActionType type, float x, float y);

    };
private:
    Vec2 speed;
    int hp;
    std::queue<Action> taskQueue;
    std::vector<std::weak_ptr<GameObject>> minionArray;
    
public:
    Alien(GameObject& associated, int nMinions);
    ~Alien();

public:
    void Start();
    void Update(float dt);
    void Render() {}

public:
    void Move(Alien::Action& act);
    void Shoot(Alien::Action& act);
public:
    bool Is(const std::string& type);
};