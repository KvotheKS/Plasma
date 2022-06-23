#pragma once

#include "../include/State.hpp"

//////////////////////////////////////////////// Game State Functions /////////////////////////////////////////////////

State::State() 
{
    this->quitRequested = false;
    LoadAssets();
}

bool State::QuitRequested() { return this->quitRequested; }

void State::LoadAssets()
{
    this->bg.Open("./resources/img/ocean.jpg");
    this->music.Open("./resources/audio/stageState.ogg");
}

void State::Update(float dt)
{
    if(SDL_QuitRequested())
        this->quitRequested = true;
}

void State::Render()
{
    this->bg.Render(0,0);
    this->music.Play();
}