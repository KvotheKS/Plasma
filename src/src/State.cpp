#pragma once

#include "../include/State.hpp"
#include "../include/Rect.hpp"
#include "../include/Sprite.hpp"
#include "../include/Sound.hpp"
#include "../include/Music.hpp"
#include "../include/Face.hpp"

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
    this->bg.AddComponents({
        spr,
        msc
    });

    this->bg.box = { 0, 0, (float)inst.GetWidth(), (float)inst.GetHeight() };
}

void State::Update(float dt)
{
    this->Input();
    
    if(SDL_QuitRequested())
    {
        this->quitRequested = true;
        return;
    }

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
}

void State::Render()
{
    if(this->quitRequested)
        return;

    this->bg.Render();
    ((Music*)this->bg.GetComponent("Music"))->Play();
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
    Game& gref = Game::GetInstance(); 
    
    const float windowWidth = gref.GetWidth(), windowHeight = gref.GetHeight();
    const float cW = spr->GetWidth(), cH = spr->GetHeight();
    
    const float cX = mouseX < 0 ? 0 : (mouseX > windowWidth - cW ? windowWidth - cW : mouseX);
    const float cY = mouseY < 0 ? 0 : (mouseY > windowHeight - cH ? windowHeight - cH : mouseY);
   
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