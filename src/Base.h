//////////////////////////// mudanca da parte 3 para a 4!
//////////////////////////// a partir de agora, as classes serao separadas
//////////////////////////// nao mais dentro da Game, mas sim de um namespace.
#pragma once

#include <./SDL2/SDL.h>
#include <./SDL2/SDL_image.h>
#include <./SDL2/SDL_mixer.h>
#include <./SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <stdexcept>

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <memory>
#include <initializer_list>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <unordered_map>


#define LEFT_ARROW_KEY    SDLK_LEFT 
#define RIGHT_ARROW_KEY   SDLK_RIGHT 
#define UP_ARROW_KEY      SDLK_UP 
#define DOWN_ARROW_KEY    SDLK_DOWN 
#define ESCAPE_KEY        SDLK_ESCAPE 
#define SPACEBAR_KEY      SDLK_SPACE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT

class Vec2;
class Rect;
class Game;
class Component;
class Sound;
class Music;
class Sprite;
class State;
class GameObject;
class Face;
class TileSet;
class TileMap;
class Resources;
class InputManager;
class Camera;
class CameraFollower;

class Game
{
////////////////////////////////////////////// Game Attributes ////////////////////////////////////////////////////

private:
    static Game* instance;
    static bool allocated;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    State* state = nullptr;

private:
    int WindowWidth, WindowHeight;

private:
    int fps;
    float period;
    int frameStart;
    float dt;

public:
    static void ThrowException()
    {
        throw std::runtime_error(SDL_GetError());
        abort();
    }

private:
    Game(const char* Title = "Titulo", int width = 600, int height = 480);

    void InitConfigs(int width, int height);

public:
    ~Game();

public:
    SDL_Renderer* GetRenderer(){ return this->renderer; }
    
    State& GetState(){ return *this->state; }
    
    static Game& GetInstance(const char* Title = "Titulo", int width = 600, int height = 480);

public:
    void CalculateDeltaTime();
    float GetDeltaTime() { return this->dt; }
public:
    int GetWidth() { return this->WindowWidth; }
    int GetHeight() { return this->WindowHeight; }

public:
    void Run();
};

///////////////////////////////////////////// Game Vector Class //////////////////////////////////////////////

class Vec2
{
public:
    float x, y;

public:
    Vec2(float x = 0.0f, float y = 0.0f);
    Vec2(const Vec2& vec);

public:
    void sum(const Vec2& vec);
    void sub(const Vec2& vec);
    void mulc(const float scalar);
    void divc(const float scalar);
    float mag();
    Vec2 norm();
    float dist(const Vec2& vec);
    float theta();
    float theta(const Vec2& vec);
    Vec2& Rotate(float angle);

public:
    static Vec2 sum(const Vec2& vecl, const Vec2& vecr);
    static Vec2 sub(const Vec2& vecl, const Vec2& vecr);
    static Vec2 mulc(const Vec2& vec, const float scalar);
    static Vec2 divc(const Vec2& vec, const float scalar);
    static float mag(const Vec2& vec);
    static Vec2 norm(const Vec2& vec);
    static float dist(const Vec2& vecl, const Vec2& vecr);
    static float theta1(const Vec2& vec);
    static float theta(const Vec2& vecl, const Vec2& vecr);
    static Vec2& Rotate(Vec2 vec,float angle)
    { return Vec2(vec).Rotate(angle); }

public:
    Vec2 operator+(const Vec2& vec) { return Vec2::sum(*this,vec); }
    Vec2 operator-(const Vec2& vec) { return Vec2::sub(*this,vec); }
    Vec2 operator*(const float scalar) { return Vec2::mulc(*this,scalar); }
    Vec2 operator/(const float scalar) { return Vec2::divc(*this, scalar); }

    Vec2& operator+=(const Vec2& vec) { this->sum(vec); return *this; }
    Vec2& operator-=(const Vec2& vec) { this->sub(vec); return *this; }
    Vec2& operator*=(const float scalar) { this->mulc(scalar); return *this; }
    Vec2& operator/=(const float scalar) { this->divc(scalar); return *this; }
};

///////////////////////////////////////////// Game Rect Class ////////////////////////////////////////////////

class Rect 
{
public:
    float x, y, w, h;

public:
    Rect(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f);
    Rect(const Rect& rect) { Rect(rect.x,rect.y,rect.w,rect.h); }

public:
    Vec2 center();
    float centerDist(const Rect& rect);
    bool in(const Vec2& vec);

public:
    Vec2 center(const Rect& rect) { return Rect(rect).center();}
    
    float centerDist(const Rect& rectl, const Rect& rectr)
    { return Rect(rectl).center().dist(Rect(rectr).center()); }

    bool in(const Rect& rect, const Vec2& vec)
    { return Rect(rect).in(vec); }

    bool intersect(const Rect& rect)
    {
        return in(Vec2(rect.x, rect.y)) ||
                in(Vec2(rect.x + rect.w, rect.y)) ||
                in(Vec2(rect.x, rect.y + rect.h)) ||
                in(Vec2(rect.x + rect.w, rect.y + rect.h));
    }

    Rect intersection(const Rect& rect);

public:
    Rect& operator=(const Rect& rect)
    { this->x = rect.x; this->y = rect.y; this->w = rect.w; this->h = rect.h; return *this;}
public:
    Rect copy() { return {this->x, this->y, this->w, this->h}; }
};

///////////////////////////////////////////// Game Component Class ///////////////////////////////////////////

class Component
{
protected:
    GameObject& associated;

public:
    Component() : associated(*(GameObject*)nullptr) { }
    Component(GameObject& associated): associated(associated) { }
    virtual ~Component() {};

public:
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

public:
    virtual bool Is(const std::string& type) = 0;
};

///////////////////////////////////////////// Game Sound Class ///////////////////////////////////////////////

class Sound : public Component
{
private:
    Mix_Chunk* chunk;
    int channel;

public:
    Sound() : Component() {}
    Sound(GameObject& associated) : Component(associated) 
    { this->chunk = nullptr; this->channel = -1; }
    Sound(GameObject& associated, const std::string& file);
    ~Sound() { this->chunk = nullptr; }

public:
    void Play(int times = 0);
    void Stop();

public:
    void Open(const std::string& file);
    bool IsOpen() { return this->chunk != nullptr; }

public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type) { return type == "Sound"; }

};

///////////////////////////////////////////// Game Music Class ///////////////////////////////////////////////

class Music : public Component
{
private:
    Mix_Music* music;
public:
    Music() : Component() { this->music = nullptr; }
    Music(GameObject& associated, const std::string& file);
    ~Music();
public:
    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(const std::string& file);
public:
    
    void Update(float dt) {}
    void Render() { Play(); }
public:
    bool IsOpen() { return this->music != nullptr; }
public:
    bool Is(const std::string& type) { return type == "Music";}
};

///////////////////////////////////////////// Game Sprite Class //////////////////////////////////////////////

class Sprite : public Component
{
private:
    SDL_Texture* texture;
    int width, height;
    SDL_Rect clipRect;

public:
    Sprite();

    Sprite(const std::string& file);

    Sprite(GameObject& associated);

    Sprite(GameObject& associated, const std::string& file);

    ~Sprite();

public:
    void Open(const std::string& file, int x, int y, int w=-1, int h=-1);

    void Open(const std::string& file);

    void SetClip(int x, int y, int w, int h);

public:
    void Update(float dt) {};
    void Render();
    void Render(float x, float y);
    void Render(Rect& toPrint);

public:
    int GetWidth(){ return this->width; }
    int GetHeight(){ return this->height; }
    bool IsOpen(){ return this->texture != nullptr; }

public:
    bool Is(const std::string& type) { return type == "Sprite"; }
};

///////////////////////////////////////////// Game GameObject Class //////////////////////////////////////////

class GameObject
{
private:
    std::vector<Component*> components;
    bool isDead;

public:
    Rect box;

public:
    GameObject();
    ~GameObject();

public:
    void Update(float dt);
    void Render();

public:
    bool IsDead() { return this->isDead; }

public:
    void RequestDelete() { this->isDead = true;}
    
    void AddComponent(Component* cpt) 
    { this->components.push_back(cpt); }
    
    void AddComponents(std::initializer_list<Component*> cpts);
    void RemoveComponent(Component* cpt);
    Component* GetComponent(const std::string& type);
};

//////////////////////////////////////////// Game State Class /////////////////////////////////////////////////

class State
{
private:
    GameObject bg;
    std::vector<std::unique_ptr<GameObject>> objectArray;
    bool quitRequested;

public:
    State();
    ~State();

public:
    void Update(float dt);
    void Render();

public:
    void LoadAssets();
    bool QuitRequested() { return this->quitRequested; }

public:
    void AddObject(int mouseX, int mouseY);
};

////////////////////////////////////////////// Game Face Class ////////////////////////////////////////////////////

class Face : public Component
{
private:
    int hitpoints;

public:
    Face() {}
    Face(GameObject& associated) : Component(associated) 
    { this->hitpoints = 30;  }
    ~Face() { }
public:
    void Damage(int damage);
    void Death();
public:
    void Update(float dt);
    void Render();

public:
    bool Is(const std::string& type) { return type == "Face";}
};
////////////////////////////////////////////// Game TileSet Class /////////////////////////////////////////////////

class TileSet
{
private:
    Sprite tileSet;
    int rows, columns;
    int tileWidth, tileHeight;

public:
    TileSet(int tileWidth, int tileHeight, const std::string& file);
    void RenderTile(unsigned index, float x, float y);

    int GetTileWidth() { return this->tileWidth; }
    int GetTileHeight() { return this->tileHeight; }
    int GetTileNumber() { return this->tileWidth*this->tileHeight; }
    int GetRows() { return this->rows; }
    int GetColumns() { return this->columns; }
};

////////////////////////////////////////////// Game Class TileMap /////////////////////////////////////////////////

class TileMap : public Component
{
private:
    std::vector<int> tileMatrix;
    TileSet* tileSet;
    int mapWidth, mapHeight, mapDepth;

public:
    TileMap() : Component() { this->tileSet = nullptr; }
    TileMap(GameObject& associated, const std::string& file, TileSet* tileSet);
    
    ~TileMap() 
    {
        if(this->tileSet)
            delete this->tileSet;
    }

public:    
    void Load(const std::string& file);
    void SetTileSet(TileSet* tileSet)
    { this->tileSet = tileSet; }

    int AtIdx(int x, int y, int z = 0)
    { return this->mapWidth*(z*this->mapHeight + y) + x; }
    
    int& At(int x, int y, int z = 0)
    { return this->tileMatrix[this->mapWidth*(z*this->mapHeight + y) + x]; }

public:
    void Render();
    void RenderLayer(int layer, const Rect& toRender, const Rect& tSetC);
    void Update(float dt) {}

public:
    bool Is(const std::string& type) { return type == "TileMap"; }

public:
    int GetWidth() { return this->mapWidth; }
    int GetHeight() { return this->mapHeight; }
    int GetDepth() { return this->mapDepth; }
    int GetPWidth() { return this->mapWidth*tileSet->GetTileWidth(); }
    int GetPHeight() { return this->mapHeight*tileSet->GetTileHeight(); }

public:
    Rect adapt(const Rect& tSetC);
    Rect tileCord(Rect& tSetC);
};

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

//////////////////////////////////////////////////////// Game InputManager class ///////////////////////////////////////////////////////////

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
    ~InputManager() {}

public:
    void Update();

//////////////////////////TO-DO Criar enum Game::Key.
public:
    bool KeyPress(int key) { return this->keyState[key] && (this->keyUpdate[key] == this->updateCounter); }
    bool KeyRelease(int key) { return !this->keyState[key] && (this->keyUpdate[key] == this->updateCounter); }
    bool IsKeyDown(int key) { return this->keyState[key]; }

    bool MousePress(int key) { return this->mouseState[key] && (this->mouseUpdate[key] == this->updateCounter); }
    bool MouseRelease(int key) { return !this->mouseState[key] && (this->mouseUpdate[key] == this->updateCounter); }
    bool IsMouseDown(int key) { return this->mouseState[key]; }

public:
    static InputManager& GetInstance()
    { static InputManager Meyer; return Meyer;}

    int GetMouseX() { return this->mouseX; }
    int GetMouseY() { return this->mouseY; }
    bool QuitRequested() { return this->quitRequested; }
};

//////////////////////////////////////////////////////////////////// Game Camera class //////////////////////////////////////////////////////////////

class Camera
{
private:
    static GameObject* focus;

public:
    static Vec2 pos, speed;

public:
    static void Follow(GameObject* newFocus) { Camera::focus = newFocus; }
    static void Unfollow() { Camera::focus = nullptr; }

public:
    static void Update(float dt);
};

////////////////////////////////////////////////////////////////// Game CameraFollower Class ///////////////////////////////////////////////////////

class CameraFollower : public Component
{
public:
    CameraFollower(GameObject& go) : Component(go) {}

public:
    void Update(float dt);
    void Render() {}

public:
    bool Is(const std::string& type) { return type == "CameraFollower"; }
};

//////////////////////////////////////// Game Vec2 Functions ///////////////////////////////////////////////

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2::Vec2(const Vec2& vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

void Vec2::sum(const Vec2& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

void Vec2::sub(const Vec2& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
}

void Vec2::mulc(const float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
}

void Vec2::divc(const float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
}

float Vec2::mag()
{
    return sqrt(this->x*this->x + this->y*this->y);
}

Vec2 Vec2::norm()
{
    Vec2 vec(this->x, this->y);
    vec.divc(vec.mag());
    return vec;
}

float Vec2::dist(const Vec2& vec)
{
    const float xl = this->x - vec.x;
    const float yl = this->y - vec.y;
    return sqrt(xl*xl + yl*yl);
}

float Vec2::theta()
{
    return atan2(this->y, this->x);
}

float Vec2::theta(const Vec2& vec)
{
    return Vec2::sub(*this, vec).theta();
}

Vec2 Vec2::sum(const Vec2& vecl, const Vec2& vecr)
{
    Vec2 rvec(vecl);
    rvec.sum(vecr);
    return rvec;
}

Vec2 Vec2::sub(const Vec2& vecl, const Vec2& vecr)
{
    Vec2 rvec(vecl);
    rvec.sub(vecr);
    return rvec;
}

Vec2 Vec2::mulc(const Vec2& vec, const float scalar)
{
    Vec2 rvec(vec);
    rvec.mulc(scalar);
    return rvec;
}

Vec2 Vec2::divc(const Vec2& vec, const float scalar)
{
    Vec2 rvec(vec);
    rvec.divc(scalar);
    return rvec;
}

float Vec2::mag(const Vec2& vec)
{
    return Vec2(vec).mag();
}

Vec2 Vec2::norm(const Vec2& vec)
{
    return Vec2(vec).norm();
}

float Vec2::dist(const Vec2& vecl, const Vec2& vecr)
{
    return Vec2(vecl).dist(vecr);
}

float Vec2::theta1(const Vec2& vec)
{
    return Vec2(vec).theta();
}

float Vec2::theta(const Vec2& vecl, const Vec2& vecr)
{
    return Vec2::sub(vecl, vecr).theta();
}

Vec2& Vec2::Rotate(float angle)
{
    const float cs = cos(angle), sn = sin(angle);
    
    const float xl = this->x*cs - this->y*sn;
    const float yl = this->y*cs + this->x*sn;

    this->x = xl;
    this->y = yl;

    return *this;
}

///////////////////////////////////////// Game Rect Functions ///////////////////////////////////////////////

Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Vec2 Rect::center()
{
    return Vec2(this->x + (this->w/2), this->y + (this->h/2));
}

float Rect::centerDist(const Rect& rect)
{
    return abs(this->center().dist(Rect(rect).center()));
}

bool Rect::in(const Vec2& vec)
{
    return (this->x <= vec.x && this->x + this->w >= vec.x) &&
           (this->y <= vec.y && this->y + this->h >= vec.y);
}

Rect Rect::intersection(const Rect& rect)
{
    
    const float xl = std::max(rect.x, this->x);    
    const float yl = std::max(rect.y, this->y);
    const float wl = std::min(rect.x+rect.w, this->x+this->w) - xl;
    const float hl = std::min(rect.y+rect.h, this->y+this->h) - yl;
    
    return {xl, yl, wl, hl};
}

/////////////////////////////////////////////// Game Base Functions /////////////////////////////////////////////////////////

Game::Game(const char* Title, int width, int height)
{
    if(!Game::instance)
        Game::instance = this;

    // Init SDL2
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        Game::ThrowException();
    }
    // Init SDL_image
    const int imgFlags = IMG_INIT_JPG  |  IMG_INIT_PNG  |  IMG_INIT_TIF; 
    
    if(IMG_Init(imgFlags) == 0)
    {
        Game::ThrowException();
    }
    // Init SDL_mixer
    const int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
    
    if(Mix_Init(mixFlags) == 0)
    {
        Game::ThrowException();
    }
    
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 1024)!=0)
    {
        Game::ThrowException();
    }

    Mix_AllocateChannels(32);

    this->window = SDL_CreateWindow(
        Title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_SHOWN
    );

    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    InitConfigs(width, height);
}

void Game::InitConfigs(int width, int height)
{
    this->fps = 30;
    this->period = 1000.0f/((float)fps);
    this->frameStart = SDL_GetTicks();
    this->dt = 0.0f;

    this->WindowHeight = height;
    this->WindowWidth = width;

    this->state = new State();
    
    srand(time(NULL));
}

Game::~Game()
{
    if(Game::instance)
    {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        
        IMG_Quit();

        Mix_CloseAudio();
        Mix_Quit();
        
        if(Game::allocated)
        {
            Game* hp = Game::instance;
            Game::allocated = false;
            Game::instance = nullptr;
            delete hp;
        }
    }
}

Game& Game::GetInstance(const char* Title, int width, int height)
{  
    if(!Game::instance)
    {
        Game::allocated = true;
        Game::instance = new Game(Title, width, height);
    }
    
    return *Game::instance;
}

void Game::CalculateDeltaTime()
{
    const int currT = SDL_GetTicks();
    dt = (currT - frameStart)/1000.0f;
    frameStart = currT;
}

void Game::Run()
{
    InputManager& controller = InputManager::GetInstance();
    while(!this->state->QuitRequested())
    {
        CalculateDeltaTime();

        controller.Update();

        this->state->Update(dt);
        
        this->state->Render();
        
        SDL_RenderPresent(this->renderer);
        
        SDL_Delay(this->period);
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}

///////////////////////////////////////// Game Component Functions //////////////////////////////////////////

////////////////////////////////////////// Game Sound Functions /////////////////////////////////////////////////

Sound::Sound(GameObject& associated, const std::string& file)
    : Component(associated)
{
    this->associated = associated; 
    this->chunk = nullptr; 
    this->channel = -1; 
    Open(file);
}

void Sound::Play(int times)
{
    if(this->channel == -1 || !Mix_Playing(this->channel))
    {
        this->channel = Mix_PlayChannel(-1, this->chunk, times);
    }
    // ################################################ botar throw exception? erro na musica deveria quebrar tudo?
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

void Sound::Update(float dt)
{ }

void Sound::Render()
{ }

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


///////////////////////////////////////////////// Game Sprites ///////////////////////////////////////////////

Sprite::Sprite() : Component()
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->clipRect = {0, 0, 0, 0};
}

Sprite::Sprite(GameObject& associated, const std::string& file)
 : Component(associated)
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    
    Open(file);
}

Sprite::~Sprite()
{
    if(this->texture)
        this->texture = nullptr;
}

void Sprite::Open(const std::string& file, int x, int y, int w, int h)
{
    Open(file);
    
    w = w == -1 ? this->width : w;
    h = h == -1 ? this->height : h;

    SetClip(x,y,w,h);
}

void Sprite::Open(const std::string& file)
{
    this->texture = Resources::GetImage(file);

    if(this->texture == nullptr)
        Game::ThrowException();
    
    SDL_QueryTexture(this->texture, nullptr, nullptr, &this->width, &this->height);
    SetClip(0,0,this->width, this->height);
}

void Sprite::SetClip(int x, int y, int w, int h)
{
    this->clipRect = {
        x, y, w, h
    };
}

void Sprite::Render()
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    const Vec2 cPos = Camera::pos;
    const Rect& wrp = this->associated.box;
    
    SDL_Rect dst = {
        (int)(wrp.x - cPos.x),
        (int)(wrp.y - cPos.y),
        (int)wrp.w,
        (int)wrp.h
    };

    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

void Sprite::Render(float x, float y)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    
    SDL_Rect dst = {
        (int)x,
        (int)y,
        this->clipRect.w, 
        this->clipRect.h
    };

    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

///////////////////////////////////////// Game GameObject Functions /////////////////////////////////////////


GameObject::GameObject()
{
    this->isDead = false;
}

GameObject::~GameObject()
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        delete (*it);
    }
    this->components.clear();
}
    
void GameObject::Update(float dt)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
        (*it)->Update(dt);
    
}

void GameObject::Render()
{
    Game& gref = Game::GetInstance();
    
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Render();
    }
}

void GameObject::RemoveComponent(Component* cpt)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if(cpt == *it)
        {
            this->components.erase(it);
            break;
        }
    }
}

void GameObject::AddComponents(std::initializer_list<Component*> cpts)
{
    for(std::initializer_list<Component*>::iterator it = cpts.begin();
                it != cpts.end(); ++it)
        this->components.push_back(*it);
}

Component* GameObject::GetComponent(const std::string& type)
{
    for(std::vector<Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if((*it)->Is(type))
            return *it;
    }

    return nullptr;
}

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
    const Vec2 cPos = Camera::pos;

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

//////////////// to-do: box, unique_ptr em GameObject.
void State::AddObject(int mouseX, int mouseY)
{
    Game& gref = Game::GetInstance(); 

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

//////////////////////////////////////////////// Game Face Functions ///////////////////////////////////////////


void Face::Damage(int damage)
{
    this->hitpoints -= damage;
    
    if(this->hitpoints <= 0)
        Death();    
}

void Face::Death()
{
    Sound* sptr = ((Sound*)this->associated.GetComponent("Sound"));
    if(sptr)
        sptr->Play();
    this->associated.RequestDelete();
}

void Face::Update(float dt) 
{
    InputManager& controller = InputManager::GetInstance();
    const Vec2 cPos = Camera::pos;

    if(controller.MousePress(LEFT_MOUSE_BUTTON) && 
        this->associated.box.in(Vec2(controller.GetMouseX() + cPos.x, controller.GetMouseY() + cPos.y)))
    {
        Damage(30);
    }
}

void Face::Render() {}

///////////////////////////////////// Game TileSet Functions ////////////////////////////

TileSet::TileSet(int tileWidth, int tileHeight, const std::string& file)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    
    this->tileSet.Open(file);
    this->rows = this->tileSet.GetHeight()/this->tileHeight;
    this->columns = this->tileSet.GetWidth()/this->tileWidth;
}

void TileSet::RenderTile(unsigned index, float x, float y)
{
    if(index >= 0 && index < this->GetTileNumber())
    {
        const int tileWN = this->columns;
        const int tileHN = this->rows;

        const int wIndex = index%tileWN;
        const int hIndex = index/tileWN;
        
        this->tileSet.SetClip(
            wIndex*this->tileWidth,
            hIndex*this->tileHeight,
            this->tileWidth,
            this->tileHeight       
        );

        this->tileSet.Render(x,y);
    }
}

////////////////////////////////////////// Game TileMap functions ////////////////////////////////

TileMap::TileMap(GameObject& associated, 
            const std::string& file, TileSet* tileSet)
    : Component(associated)
{
    Load(file);
    SetTileSet(tileSet);
}
    
void TileMap::Load(const std::string& file)
{
    FILE* fl = fopen(file.c_str(), "r");
    
    fscanf(fl, "%d,%d,%d,", &this->mapWidth, &this->mapHeight, &this->mapDepth);

    this->tileMatrix.resize(this->mapWidth*this->mapHeight*this->mapDepth);
    
    int tmp;
    const int vSize = this->mapHeight*this->mapWidth*this->mapDepth;
    
    for(int i = 0; i < vSize; i++)
    {
        fscanf(fl, "%d,", &tmp);
        this->tileMatrix[i] = tmp - 1; 
    } 
}

Rect TileMap::adapt(const Rect& tSetC)
{
    const float tW = this->tileSet->GetTileWidth();
    const float tH = this->tileSet->GetTileHeight();
    
    const float xl = floor(tSetC.x/tW)*tW;
    const float yl = floor(tSetC.y/tH)*tH;

    const float wl = ceil((tSetC.x+tSetC.w)/tW)*tW - xl;
    const float hl = ceil((tSetC.y+tSetC.h)/tH)*tH - yl;
    
    return {xl,yl,wl,hl};
}

Rect TileMap::tileCord(Rect& tSetC)
{
    /*
        Retorna as coordenadas do primeiro tile a ser renderizado e tambem
        as do ultimo. Assume que tSetC esta alinhado com o tileset e associated.box
    */
    const float tW = this->tileSet->GetTileWidth();
    const float tH = this->tileSet->GetTileHeight();

    const float xl = tSetC.x/tW;
    const float yl = tSetC.y/tH;

    const float x2l = (tSetC.x + tSetC.w)/tW;
    const float y2l = (tSetC.y + tSetC.h)/tH;

    return {xl,yl,x2l,y2l};
}

void TileMap::Render()
{
    Game& inst = Game::GetInstance();
    const Vec2 cPos = Camera::pos;
    int cameraX = cPos.x, cameraY = cPos.y;

    Rect gCamera = { (float)cameraX, (float)cameraY, 
                           (float)inst.GetWidth(), (float)inst.GetHeight() };

    if(!this->associated.box.intersect(gCamera))
        return;
    
    Rect toRender = adapt(gCamera.intersection(this->associated.box.copy()));
    Rect tSetC = tileCord(toRender);
    
    toRender.x -= cameraX; toRender.y -= cameraY;
    
    for(int i = 0; i < this->mapDepth; i++)
        RenderLayer(i, toRender, tSetC);
}

void TileMap::RenderLayer(int layer, const Rect& toRender, const Rect& tSetC)
{
    Game& inst = Game::GetInstance();
    
    const int tW = this->tileSet->GetTileWidth();
    const int tH = this->tileSet->GetTileHeight();

    const int fs = AtIdx(tSetC.x, tSetC.y, layer);

    const int nxt = AtIdx(tSetC.x, tSetC.y+1, layer) - AtIdx(tSetC.w, tSetC.y, layer);

    int x = toRender.x, y = toRender.y;

    for(int i = fs, y = toRender.y; y < toRender.y + toRender.h; y+=tH, i+=nxt)
        for(int x = toRender.x; x < toRender.x + toRender.w; i++, x+=tW)
        {
            if(this->tileMatrix[i] != -1)
                this->tileSet->RenderTile(this->tileMatrix[i], x, y);
            
        }
}

/////////////////////////////////////////////////// Game Resources Functions ///////////////////////////////////////////

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

///////////////////////////////////////// Game InputManager Functions ///////////////////////////////

InputManager::InputManager()
    : quitRequested(false), mouseState{false},
    mouseUpdate{0}, updateCounter(0), mouseX(0), mouseY(0)
{}

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

////////////////////////////////////////////////////////////// Game Camera functions ////////////////////////////////////////////////////////

void Camera::Update(float dt)
{
    InputManager& controller = InputManager::GetInstance();
    if(Camera::focus)
        Camera::pos = Camera::focus->box.center();
    else
    {
        Camera::pos.x += (controller.IsKeyDown(RIGHT_ARROW_KEY) - controller.IsKeyDown(LEFT_ARROW_KEY))*speed.x;
        Camera::pos.y += (controller.IsKeyDown(DOWN_ARROW_KEY) - controller.IsKeyDown(UP_ARROW_KEY))*speed.y;
        
        Camera::pos.x = std::max(Camera::pos.x, 0.0f);
        Camera::pos.y = std::max(Camera::pos.y, 0.0f);
    }
}

/////////////////////////////////////////////////////////// Game CameraFollower functions //////////////////////////////////////////////////////////////

void CameraFollower::Update(float dt) 
{
    const Vec2 cPos = Camera::pos;
    this->associated.box.x = cPos.x;
    this->associated.box.y = cPos.y;
}