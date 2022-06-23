#pragma once

#include "../include/Component.hpp"
#include "../include/Sound.hpp"
#include "../include/Game.hpp"
#include "../include/Resources.hpp"

Sound::Sound() : Component() {}

Sound::Sound(GameObject& associated) : Component(associated) 
{ this->chunk = nullptr; this->channel = -1; }

Sound::~Sound() 
{ this->chunk = nullptr; }

Sound::Sound(GameObject& associated, const std::string& file)
    : Component(associated)
{
    this->chunk = nullptr;
    this->channel = -1; 
    Open(file);
}

void Sound::Play(int times)
{
    if(this->channel == -1 || !Mix_Playing(this->channel))
        this->channel = Mix_PlayChannel(-1, this->chunk, times);
}

void Sound::Stop()
{
    if(this->channel != -1 && this->chunk != nullptr)
    {
        Mix_HaltChannel(this->channel);
        this->channel = -1;
    }
}

void Sound::Open(const std::string& file)
{
    this->chunk = Resources::GetSound(file);
}

bool Sound::IsOpen() { return this->chunk != nullptr; }

bool Sound::IsPlaying() 
{ 
    const bool tmp = Mix_Playing(this->channel); 
    if(!tmp) this->channel = -1; 
    return tmp;
}

void Sound::Update(float dt)
{ }

void Sound::Render()
{ }

bool Sound::Is(const std::string& type) 
{ return type == "Sound"; }