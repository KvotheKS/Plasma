#pragma once

#include "../include/Music.hpp"
#include "../include/Game.hpp"
#include "../include/Resources.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

////////////////////////////////////////// Game Music Functions ////////////////////////////////////////////////
Music::Music(GameObject& associated, const std::string& file)
    : Component(associated)
{
    Open(file);
}

Music::~Music()
{
    if(this->music)
        Stop();
      
}

void Music::Open(const std::string& file)
{
    this->music = Resources::GetMusic(file);
}

void Music::Play(int times)
{
    if(!this->music)
        Game::ThrowException();
    if(!Mix_PlayingMusic())
        Mix_PlayMusic(this->music, times);
}

void Music::Stop(int msToStop)
{
    Mix_FadeOutMusic(msToStop);
}

void Music::Update(float dt) 
{}
void Music::Render() {}


bool Music::IsOpen() { return this->music != nullptr; }
bool Music::Is(const std::string& type) { return type == "Music"; }