#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
//#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#define LEFT_ARROW_KEY    SDLK_LEFT 
#define RIGHT_ARROW_KEY   SDLK_RIGHT 
#define UP_ARROW_KEY      SDLK_UP 
#define DOWN_ARROW_KEY    SDLK_DOWN 
#define ESCAPE_KEY        SDLK_ESCAPE 
#define SPACEBAR_KEY      SDLK_SPACE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON   SDL_BUTTON_RIGHT

#include "./decl.hpp"
#include <unordered_map>

class InputManager
{
private:
    bool mouseState[6];
    int mouseUpdate[6];
    bool quitRequested;
    int updateCounter;
    int mouseX, mouseY;
    std::unordered_map<int,bool> keyState;
    std::unordered_map<int,int> keyUpdate;

private:
    InputManager();
    ~InputManager();

public:
    void Update();

//////////////////////////TO-DO Criar enum Game::Key.
public:
    bool KeyPress(int key);
    bool KeyRelease(int key);
    bool IsKeyDown(int key);

    bool MousePress(int key);    
	bool MouseRelease(int key);
    bool IsMouseDown(int key);

public:
    static InputManager& GetInstance();

    int GetMouseX();
    int GetMouseY();
    bool QuitRequested();
};