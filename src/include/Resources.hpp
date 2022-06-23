#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "./decl.hpp"
#include <unordered_map>
#include <string>

class Resources
{
	private:
        static std::unordered_map<std::string, SDL_Texture*> imageTable;
        static std::unordered_map<std::string, Mix_Music*> musicTable;
        static std::unordered_map<std::string, Mix_Chunk*> soundTable;
    
    public:
        static SDL_Texture* GetImage(const std::string&  file);
        static Mix_Music* GetMusic(const std::string&  file);
        static Mix_Chunk* GetSound(const std::string&  file);
    
    public:
        static void ClearImages();
        static void ClearMusics();
        static void ClearSounds();
};