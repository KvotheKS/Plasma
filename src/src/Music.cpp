#pragma once

#include "../include/Music.hpp"
#include "../include/Game.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

////////////////////////////////////////// Game Music Functions ////////////////////////////////////////////////
Music::Music() { this->music = nullptr; }

Music::Music(const std::string& file)
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

bool Music::IsOpen() { return this->music != nullptr; }
