#pragma once

#define _USE_MATH_DEFINES
#include "../include/State.hpp"
#include "../include/Rect.hpp"
#include "../include/Sprite.hpp"
#include "../include/Sound.hpp"
#include "../include/Music.hpp"
#include "../include/Face.hpp"
#include "../include/TileSet.hpp"
#include "../include/TileMap.hpp"
#include "../include/GameObject.hpp"
#include "../include/CameraFollower.hpp"
#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"

//////////////////////////////////////////////// Game State Functions /////////////////////////////////////////////////

State::State() 
{
    this->quitRequested = false;
    LoadAssets();
}

State::~State()
{
    this->objectArray.clear();
}

void State::LoadAssets()
{
    Game& inst = Game::GetInstance();
    Sprite* spr = new Sprite(this->bg, "./resources/img/ocean.jpg");
    Music* msc = new Music(this->bg, "./resources/audio/stageState.ogg");
    msc->Play();
    this->bg.AddComponents({
        spr,
        msc,
        new CameraFollower(this->bg)
    });
    
    this->bg.box = { 0, 0, (float)inst.GetWidth(), (float)inst.GetHeight() };

    GameObject* go = new GameObject();
    TileMap* map = new TileMap(
        *go , 
        "./resources/map/tileMap.txt", 
        new TileSet(64, 64, "./resources/img/tileSet.png")
    );

    go->box = { 0, 0, (float)map->GetPWidth(), (float)map->GetPHeight() };
    
    go->AddComponent(map);
    this->objectArray.push_back(std::unique_ptr<GameObject>(go));
}

void State::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();
    
    if(SDL_QuitRequested() || controller.QuitRequested())
    {
        this->quitRequested = true;
        return;
    }

    Camera::Update(dt);
    
    if(controller.KeyPress(SPACEBAR_KEY))
        this->AddObject(controller.GetMouseX(), controller.GetMouseY());

    
    int vSize = this->objectArray.size();
    
    for(int i = 0; i < vSize; i++)
    {
        if(this->objectArray[i]->IsDead())
        {
            this->objectArray.erase(this->objectArray.begin() + i);    
            vSize--;
            i--;
        }
    }

    this->bg.Update(dt);
    
    vSize = this->objectArray.size();
    
    for(int i = 0; i < vSize; i++)
        this->objectArray[i]->Update(dt);
}

void State::Render()
{
    if(this->quitRequested)
        return;

    this->bg.Render();
    for(std::vector<std::unique_ptr<GameObject>>::iterator it = this->objectArray.begin();
            it != this->objectArray.end(); ++it)
        (*it)->Render();
}

void State::Input() {
    SDL_Event event;
    int mouseX, mouseY;

    // Obtenha as coordenadas do mouse
    SDL_GetMouseState(&mouseX, &mouseY);

    // SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
    while (SDL_PollEvent(&event)) 
    {

        // Se o evento for quit, setar a flag para terminação
        if(event.type == SDL_QUIT) 
        {
            this->quitRequested = true;
        }
        
        // Se o evento for clique...
        if(event.type == SDL_MOUSEBUTTONDOWN) 
        {

            // Percorrer de trás pra frente pra sempre clicar no objeto mais de cima
            for(int i = this->objectArray.size() - 1; i >= 0; --i) 
            {
                // Obtem o ponteiro e casta pra Face.
                GameObject* go = (GameObject*) this->objectArray[i].get();
                // Nota: Desencapsular o ponteiro é algo que devemos evitar ao máximo.
                // O propósito do unique_ptr é manter apenas uma cópia daquele ponteiro,
                // ao usar get(), violamos esse princípio e estamos menos seguros.
                // Esse código, assim como a classe Face, é provisório. Futuramente, para
                // chamar funções de GameObjects, use objectArray[i]->função() direto.

                if(go->box.in( {(float)mouseX, (float)mouseY} ) ) 
                {
                    Face* face = (Face*)go->GetComponent( "Face" );
                    if(face) 
                    {
                        // Aplica dano
                        face->Damage(std::rand() % 10 + 10);
                        // Sai do loop (só queremos acertar um)
                        break;
                    }
                }
            }
        }
        if( event.type == SDL_KEYDOWN ) 
        {
            // Se a tecla for ESC, setar a flag de quit
            if( event.key.keysym.sym == SDLK_ESCAPE ) 
            {
                this->quitRequested = true;
            }
            // Se não, crie um objeto
            else 
            {
                std::cout << mouseX << '|' << mouseY << '\n';
                Vec2 objPos = Vec2( 200, 0 ).Rotate( -M_PI + M_PI*(rand() % 1001)/500.0 ) + Vec2( mouseX, mouseY );
                AddObject((int)objPos.x, (int)objPos.y);
            }
        }
    }
}

//////////////// to-do: box, unique_ptr em GameObject.
void State::AddObject(int mouseX, int mouseY)
{
    this->objectArray.push_back(std::unique_ptr<GameObject>(new GameObject()));
    const int vSize = this->objectArray.size() - 1;
    
    Sprite* spr = new Sprite(*this->objectArray[vSize], "resources/img/penguinface.png");
    
    const Vec2 cPos = Camera::pos;
    const float cW = spr->GetWidth(), cH = spr->GetHeight();
    
    Vec2 objPos = Vec2( 200, 0 ).Rotate( -M_PI + M_PI*(rand() % 1001)/500.0 ) + Vec2( mouseX, mouseY );

    const float cX = objPos.x + cPos.x < 0 ? 0 : objPos.x + cPos.x;
    const float cY = objPos.y + cPos.y < 0 ? 0 : objPos.y + cPos.y;
   
    this->objectArray[vSize]->box = {
        cX,
        cY,
        cW,
        cH
    };
    
    this->objectArray[vSize]->AddComponents({
        spr,
        new Sound(*this->objectArray[vSize], "resources/audio/boom.wav"),
        new Face(*this->objectArray[vSize])
    });
}

bool State::QuitRequested() { return this->quitRequested; }