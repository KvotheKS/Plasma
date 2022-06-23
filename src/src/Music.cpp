#pragma once

#include "../include/Music.hpp"
#include "../include/Game.hpp"

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
    {
        Stop();
        Mix_FreeMusic(this->music);
    }
}

void Music::Open(const std::string& file)
{
    this->music = Mix_LoadMUS(file.c_str());
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

void Music::Update(float dt) {}
void Music::Render() {}


bool Music::IsOpen() { return this->music != nullptr; }
bool Music::Is(const std::string& type) { return type == "Music"; }