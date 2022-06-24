#pragma once

#include "../include/Resources.hpp"
#include "../include/Game.hpp"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;

SDL_Texture* Resources::GetImage(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, SDL_Texture*>::iterator it = imageTable.find(file);

    if(it == imageTable.end())
        it = imageTable.insert({file, IMG_LoadTexture(renderer, file.c_str())}).first;
    
    return it->second;
}

Mix_Music* Resources::GetMusic(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, Mix_Music*>::iterator it = musicTable.find(file);
    
    if(it == musicTable.end())
        it = musicTable.insert({file, Mix_LoadMUS(file.c_str())}).first;
    
    return it->second;
}

Mix_Chunk* Resources::GetSound(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, Mix_Chunk*>::iterator it = soundTable.find(file);
    
    if(it == soundTable.end())
        it = soundTable.insert({file, Mix_LoadWAV(file.c_str())}).first;
    
    return it->second;
}

void Resources::ClearImages() 
{ 
    for(std::unordered_map<std::string, SDL_Texture*>::iterator it = imageTable.begin();
                it != imageTable.end(); it++)
        SDL_DestroyTexture(it->second);

    imageTable.clear();
}

void Resources::ClearMusics() 
{ 
    for(std::unordered_map<std::string, Mix_Music*>::iterator it = musicTable.begin();
                it != musicTable.end(); it++)
        Mix_FreeMusic(it->second);
        
    musicTable.clear();
}

void Resources::ClearSounds() 
{ 
    for(std::unordered_map<std::string, Mix_Chunk*>::iterator it = soundTable.begin();
                it != soundTable.end(); it++)
        Mix_FreeChunk(it->second);
        
    soundTable.clear();
}