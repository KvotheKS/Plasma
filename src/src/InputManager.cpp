#pragma once

#include "../include/InputManager.hpp"

InputManager::InputManager()
    : quitRequested(false), mouseState{false},
    mouseUpdate{0}, updateCounter(0), mouseX(0), mouseY(0)
{}

InputManager::~InputManager() {}

void InputManager::Update()
{
    SDL_Event event; 
    SDL_GetMouseState(&this->mouseX, &this->mouseY);
    this->quitRequested = false;
    this->updateCounter++;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                if(event.key.repeat != 1)
                {
                    keyState[event.key.keysym.sym] = true;
                    keyUpdate[event.key.keysym.sym]= this->updateCounter;
                }
            break;
            
            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;
                keyUpdate[event.key.keysym.sym]= this->updateCounter;
            break;

            case SDL_MOUSEBUTTONDOWN:
                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button]= this->updateCounter;
            break;

            case SDL_MOUSEBUTTONUP:
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button]= this->updateCounter;
            break;
            
            case SDL_QUIT:
                this->quitRequested = true;
            break;
        }
    }
}


bool InputManager::KeyPress(int key) { return this->keyState[key] && (this->keyUpdate[key] == this->updateCounter); }
bool InputManager::KeyRelease(int key) { return !this->keyState[key] && (this->keyUpdate[key] == this->updateCounter); }
bool InputManager::IsKeyDown(int key) { return this->keyState[key]; }

bool InputManager::MousePress(int key) { return this->mouseState[key] && (this->mouseUpdate[key] == this->updateCounter); }
bool InputManager::MouseRelease(int key) { return !this->mouseState[key] && (this->mouseUpdate[key] == this->updateCounter); }
bool InputManager::IsMouseDown(int key) { return this->mouseState[key]; }


InputManager& InputManager::GetInstance()
{ static InputManager Meyer; return Meyer;}

int InputManager::GetMouseX() { return this->mouseX; }
int InputManager::GetMouseY() { return this->mouseY; }
bool InputManager::QuitRequested() { return this->quitRequested; }