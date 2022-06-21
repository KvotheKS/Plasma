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

class Game
{
public:
    class Vec2;
    class Rect;
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

public:

///////////////////////////////////////////// Game Vector Class //////////////////////////////////////////////

    class Vec2
    {
    public:
        float x, y;
    
    public:
        Vec2(float x = 0.0f, float y = 0.0f);
        Vec2(const Game::Vec2& vec);

    public:
        void sum(const Game::Vec2& vec);
        void sub(const Game::Vec2& vec);
        void mulc(const float scalar);
        void divc(const float scalar);
        float mag();
        Game::Vec2 norm();
        float dist(const Game::Vec2& vec);
        float theta();
        float theta(const Game::Vec2& vec);
        Game::Vec2& Rotate(float angle);

    public:
        static Game::Vec2 sum(const Game::Vec2& vecl, const Game::Vec2& vecr);
        static Game::Vec2 sub(const Game::Vec2& vecl, const Game::Vec2& vecr);
        static Game::Vec2 mulc(const Game::Vec2& vec, const float scalar);
        static Game::Vec2 divc(const Game::Vec2& vec, const float scalar);
        static float mag(const Game::Vec2& vec);
        static Game::Vec2 norm(const Game::Vec2& vec);
        static float dist(const Game::Vec2& vecl, const Game::Vec2& vecr);
        static float theta1(const Game::Vec2& vec);
        static float theta(const Game::Vec2& vecl, const Game::Vec2& vecr);
        static Game::Vec2& Rotate(Game::Vec2 vec,float angle)
        { return Game::Vec2(vec).Rotate(angle); }

    public:
        Game::Vec2 operator+(const Game::Vec2& vec) { return Game::Vec2::sum(*this,vec); }
        Game::Vec2 operator-(const Game::Vec2& vec) { return Game::Vec2::sub(*this,vec); }
        Game::Vec2 operator*(const float scalar) { return Game::Vec2::mulc(*this,scalar); }
        Game::Vec2 operator/(const float scalar) { return Game::Vec2::divc(*this, scalar); }

        Game::Vec2& operator+=(const Game::Vec2& vec) { this->sum(vec); return *this; }
        Game::Vec2& operator-=(const Game::Vec2& vec) { this->sub(vec); return *this; }
        Game::Vec2& operator*=(const float scalar) { this->mulc(scalar); return *this; }
        Game::Vec2& operator/=(const float scalar) { this->divc(scalar); return *this; }
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
        Game::Vec2 center();
        float centerDist(const Game::Rect& rect);
        bool in(const Game::Vec2& vec);
    
    public:
        Game::Vec2 center(const Game::Rect& rect) { return Game::Rect(rect).center();}
        
        float centerDist(const Game::Rect& rectl, const Game::Rect& rectr)
        { return Game::Rect(rectl).center().dist(Game::Rect(rectr).center()); }

        bool in(const Game::Rect& rect, const Game::Vec2& vec)
        { return Game::Rect(rect).in(vec); }

        bool intersect(const Game::Rect& rect)
        {
            return in(Game::Vec2(rect.x, rect.y)) ||
                   in(Game::Vec2(rect.x + rect.w, rect.y)) ||
                   in(Game::Vec2(rect.x, rect.y + rect.h)) ||
                   in(Game::Vec2(rect.x + rect.w, rect.y + rect.h));
        }

        Game::Rect intersection(const Game::Rect& rect);
    
    public:
        Game::Rect copy() { return {this->x, this->y, this->w, this->h}; }
    };

///////////////////////////////////////////// Game Component Class ///////////////////////////////////////////

    class Component
    {
    protected:
        Game::GameObject& associated;

    public:
        Component() : associated(*(Game::GameObject*)nullptr) { }
        Component(Game::GameObject& associated): associated(associated) { }
        virtual ~Component() {};
    
    public:
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
    
    public:
        virtual bool Is(const std::string& type) = 0;
    };

///////////////////////////////////////////// Game Sound Class ///////////////////////////////////////////////

    class Sound : public Game::Component
    {
    private:
        Mix_Chunk* chunk;
        int channel;

    public:
        Sound() : Component() {}
        Sound(Game::GameObject& associated) : Component(associated) 
        { this->chunk = nullptr; this->channel = -1; }
        Sound(Game::GameObject& associated, const std::string& file);
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

    class Music : public Game::Component
    {
    private:
        Mix_Music* music;
    public:
        Music() : Component() { this->music = nullptr; }
        Music(Game::GameObject& associated, const std::string& file);
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

    class Sprite : public Game::Component
    {
    private:
        SDL_Texture* texture;
        int width, height;
        SDL_Rect clipRect;

    public:
        Sprite();

        Sprite(const std::string& file);

        Sprite(Game::GameObject& associated);

        Sprite(Game::GameObject& associated, const std::string& file);

        ~Sprite();
    
    public:
        void Open(const std::string& file, int x, int y, int w=-1, int h=-1);

        void Open(const std::string& file);

        void SetClip(int x, int y, int w, int h);
    
    public:
        void Update(float dt) {};
        void Render();
        void Render(float x, float y);
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
        std::vector<Game::Component*> components;
        bool isDead;
    
    public:
        Game::Rect box;

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
        
        void AddComponent(Game::Component* cpt) 
        { this->components.push_back(cpt); }
        
        void AddComponents(std::initializer_list<Game::Component*> cpts);
        void RemoveComponent(Game::Component* cpt);
        Game::Component* GetComponent(const std::string& type);
    };

//////////////////////////////////////////// Game State Class /////////////////////////////////////////////////

    class State
    {
    private:
        Game::GameObject bg;
        std::vector<std::unique_ptr<Game::GameObject>> objectArray;
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
        void Input();
        void AddObject(int mouseX, int mouseY);
    };

////////////////////////////////////////////// Game Face Class ////////////////////////////////////////////////////

    class Face : public Game::Component
    {
    private:
        int hitpoints;
    
    public:
        Face() {}
        Face(Game::GameObject& associated) : Component(associated) 
        { this->hitpoints = 30; }
        ~Face() {}
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
        Game::Sprite tileSet;
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

    class TileMap : public Game::Component
    {
    private:
        std::vector<int> tileMatrix;
        Game::TileSet* tileSet;
        int mapWidth, mapHeight, mapDepth;

    public:
        TileMap() : Component() { this->tileSet = nullptr; }
        TileMap(Game::GameObject& associated, const std::string& file, Game::TileSet* tileSet);
        
        ~TileMap() 
        {
            if(this->tileSet)
                delete this->tileSet;
        }

    public:    
        void Load(const std::string& file);
        void SetTileSet(Game::TileSet* tileSet)
        { this->tileSet = tileSet; }

        int AtIdx(int x, int y, int z = 0)
        { return this->mapWidth*(z*this->mapHeight + y) + x; }
        
        int& At(int x, int y, int z = 0)
        { return this->tileMatrix[this->mapWidth*(z*this->mapHeight + y) + x]; }
    
    public:
        void Render();
        void RenderLayer(int layer, const Game::Rect& toRender, const Game::Rect& tSetC);
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
        Game::Rect adapt(const Game::Rect& tSetC);
        Game::Rect tileCord(Game::Rect& tSetC);
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
    unsigned long long initialTick, lastTick, currTick;

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
    int GetWidth() { return this->WindowWidth; }
    int GetHeight() { return this->WindowHeight; }

public:
    void Run();
};

//////////////////////////////////////// Game Vec2 Functions ///////////////////////////////////////////////

Game::Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Game::Vec2::Vec2(const Game::Vec2& vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

void Game::Vec2::sum(const Game::Vec2& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

void Game::Vec2::sub(const Game::Vec2& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
}

void Game::Vec2::mulc(const float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
}

void Game::Vec2::divc(const float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
}

float Game::Vec2::mag()
{
    return sqrt(this->x*this->x + this->y*this->y);
}

Game::Vec2 Game::Vec2::norm()
{
    Game::Vec2 vec(this->x, this->y);
    vec.divc(vec.mag());
    return vec;
}

float Game::Vec2::dist(const Game::Vec2& vec)
{
    const float xl = this->x - vec.x;
    const float yl = this->y - vec.y;
    return sqrt(xl*xl + yl*yl);
}

float Game::Vec2::theta()
{
    return atan2(this->y, this->x);
}

float Game::Vec2::theta(const Game::Vec2& vec)
{
    return Game::Vec2::sub(*this, vec).theta();
}

Game::Vec2 Game::Vec2::sum(const Game::Vec2& vecl, const Game::Vec2& vecr)
{
    Game::Vec2 rvec(vecl);
    rvec.sum(vecr);
    return rvec;
}

Game::Vec2 Game::Vec2::sub(const Game::Vec2& vecl, const Game::Vec2& vecr)
{
    Game::Vec2 rvec(vecl);
    rvec.sub(vecr);
    return rvec;
}

Game::Vec2 Game::Vec2::mulc(const Game::Vec2& vec, const float scalar)
{
    Game::Vec2 rvec(vec);
    rvec.mulc(scalar);
    return rvec;
}

Game::Vec2 Game::Vec2::divc(const Game::Vec2& vec, const float scalar)
{
    Game::Vec2 rvec(vec);
    rvec.divc(scalar);
    return rvec;
}

float Game::Vec2::mag(const Game::Vec2& vec)
{
    return Game::Vec2(vec).mag();
}

Game::Vec2 Game::Vec2::norm(const Game::Vec2& vec)
{
    return Game::Vec2(vec).norm();
}

float Game::Vec2::dist(const Game::Vec2& vecl, const Game::Vec2& vecr)
{
    return Game::Vec2(vecl).dist(vecr);
}

float Game::Vec2::theta1(const Game::Vec2& vec)
{
    return Game::Vec2(vec).theta();
}

float Game::Vec2::theta(const Game::Vec2& vecl, const Game::Vec2& vecr)
{
    return Game::Vec2::sub(vecl, vecr).theta();
}

Game::Vec2& Game::Vec2::Rotate(float angle)
{
    const float cs = cos(angle), sn = sin(angle);
    
    const float xl = this->x*cs - this->y*sn;
    const float yl = this->y*cs + this->x*sn;

    this->x = xl;
    this->y = yl;

    return *this;
}

///////////////////////////////////////// Game Rect Functions ///////////////////////////////////////////////

Game::Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Game::Vec2 Game::Rect::center()
{
    return Game::Vec2(this->x + (this->w/2), this->y + (this->h/2));
}

float Game::Rect::centerDist(const Game::Rect& rect)
{
    return abs(this->center().dist(Game::Rect(rect).center()));
}

bool Game::Rect::in(const Game::Vec2& vec)
{
    return (this->x <= vec.x && this->x + this->w >= vec.x) &&
           (this->y <= vec.y && this->y + this->h >= vec.y);
}

Game::Rect Game::Rect::intersection(const Game::Rect& rect)
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
    this->initialTick = SDL_GetTicks64();
    this->lastTick = this->initialTick;
    this->currTick = this->initialTick;

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

void Game::Run()
{
    while(!this->state->QuitRequested())
    {
        const float dt = ((float)(this->currTick - this->lastTick))/100.0f;
        this->lastTick = this->currTick;   
        this->currTick = SDL_GetTicks64();
        
        this->state->Update(dt);
        
        this->state->Render();
        
        SDL_RenderPresent(this->renderer);
        
        SDL_Delay(this->period);
    }
    Game::Resources::ClearImages();
    Game::Resources::ClearMusics();
    Game::Resources::ClearSounds();
}

///////////////////////////////////////// Game Component Functions //////////////////////////////////////////

////////////////////////////////////////// Game Sound Functions /////////////////////////////////////////////////

Game::Sound::Sound(Game::GameObject& associated, const std::string& file)
    : Component(associated)
{
    this->associated = associated; 
    this->chunk = nullptr; 
    this->channel = -1; 
    Open(file);
}

void Game::Sound::Play(int times)
{
    if(this->channel == -1 || !Mix_Playing(this->channel))
    {
        this->channel = Mix_PlayChannel(-1, this->chunk, times);
    }
    // ################################################ botar throw exception? erro na musica deveria quebrar tudo?
}

void Game::Sound::Stop()
{
    if(this->channel != -1 && this->chunk != nullptr)
    {
        
        Mix_HaltChannel(this->channel);
        this->channel = -1;
    }
}

void Game::Sound::Open(const std::string& file)
{ 
    this->chunk = Game::Resources::GetSound(file);
}

void Game::Sound::Update(float dt)
{ }

void Game::Sound::Render()
{ }

////////////////////////////////////////// Game Music Functions ////////////////////////////////////////////////

Game::Music::Music(Game::GameObject& associated, const std::string& file)
    : Component(associated)
{
    Open(file);
}

Game::Music::~Music()
{
    if(this->music)
        Stop();
}

void Game::Music::Open(const std::string& file)
{
    this->music = Game::Resources::GetMusic(file);
}

void Game::Music::Play(int times)
{
    if(!this->music)
        Game::ThrowException();
    if(!Mix_PlayingMusic())
        Mix_PlayMusic(this->music, times);
}

void Game::Music::Stop(int msToStop)
{
    Mix_FadeOutMusic(msToStop);
}


///////////////////////////////////////////////// Game Sprites ///////////////////////////////////////////////

Game::Sprite::Sprite() : Component()
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->clipRect = {0, 0, 0, 0};
}

Game::Sprite::Sprite(Game::GameObject& associated, const std::string& file)
 : Component(associated)
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    
    Open(file);
}

Game::Sprite::~Sprite()
{
    if(this->texture)
        this->texture = nullptr;
}

void Game::Sprite::Open(const std::string& file, int x, int y, int w, int h)
{
    Open(file);
    
    w = w == -1 ? this->width : w;
    h = h == -1 ? this->height : h;

    SetClip(x,y,w,h);
}

void Game::Sprite::Open(const std::string& file)
{
    this->texture = Game::Resources::GetImage(file);

    if(this->texture == nullptr)
        Game::ThrowException();
    
    SDL_QueryTexture(this->texture, nullptr, nullptr, &this->width, &this->height);
    SetClip(0,0,this->width, this->height);
}

void Game::Sprite::SetClip(int x, int y, int w, int h)
{
    this->clipRect = {
        x, y, w, h
    };
}

void Game::Sprite::Render()
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    const Game::Rect& wrp = this->associated.box;
    SDL_Rect dst = {
        (int)wrp.x,
        (int)wrp.y,
        (int)wrp.w,
        (int)wrp.h
    };
    
    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

void Game::Sprite::Render(float x, float y)
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


Game::GameObject::GameObject()
{
    this->isDead = false;
}

Game::GameObject::~GameObject()
{
    for(std::vector<Game::Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        delete (*it);
    }
    this->components.clear();
}
    
void Game::GameObject::Update(float dt)
{
    for(std::vector<Game::Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Update(dt);
    }
}

void Game::GameObject::Render()
{
    for(std::vector<Game::Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        (*it)->Render();
    }
}

void Game::GameObject::RemoveComponent(Game::Component* cpt)
{
    for(std::vector<Game::Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if(cpt == *it)
        {
            this->components.erase(it);
            break;
        }
    }
}

void Game::GameObject::AddComponents(std::initializer_list<Game::Component*> cpts)
{
    for(std::initializer_list<Game::Component*>::iterator it = cpts.begin();
                it != cpts.end(); ++it)
        this->components.push_back(*it);
}

Game::Component* Game::GameObject::GetComponent(const std::string& type)
{
    for(std::vector<Game::Component*>::iterator it = this->components.begin(); 
                    it != this->components.end(); ++it)
    {
        if((*it)->Is(type))
            return *it;
    }

    return nullptr;
}

//////////////////////////////////////////////// Game State Functions /////////////////////////////////////////////////

Game::State::State() 
{
    this->quitRequested = false;
    LoadAssets();
}

Game::State::~State()
{
    this->objectArray.clear();
}

void Game::State::LoadAssets()
{
    Game& inst = Game::GetInstance();
    Sprite* spr = new Sprite(this->bg, "./resources/img/ocean.jpg");
    Music* msc = new Music(this->bg, "./resources/audio/stageState.ogg");
    
    this->bg.AddComponents({
        spr,
       msc,
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
    this->objectArray.push_back(std::unique_ptr<Game::GameObject>(go));
}

void Game::State::Update(float dt)
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

void Game::State::Render()
{
    if(this->quitRequested)
        return;

    this->bg.Render();
    for(std::vector<std::unique_ptr<Game::GameObject>>::iterator it = this->objectArray.begin();
            it != this->objectArray.end(); ++it)
        (*it)->Render();
}

void Game::State::Input() {
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
                Game::Vec2 objPos = Game::Vec2( 200, 0 ).Rotate( -M_PI + M_PI*(rand() % 1001)/500.0 ) + Game::Vec2( mouseX, mouseY );
				AddObject((int)objPos.x, (int)objPos.y);
			}
		}
	}
}

//////////////// to-do: box, unique_ptr em GameObject.
void Game::State::AddObject(int mouseX, int mouseY)
{
    Game& gref = Game::GetInstance(); 

    this->objectArray.push_back(std::unique_ptr<Game::GameObject>(new Game::GameObject()));
    const int vSize = this->objectArray.size() - 1;
    
    Game::Sprite* spr = new Sprite(*this->objectArray[vSize], "resources/img/penguinface.png");
    
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

//////////////////////////////////////////////// Game Face Functions ///////////////////////////////////////////


void Game::Face::Damage(int damage)
{
    this->hitpoints -= damage;
    
    if(this->hitpoints <= 0)
        Death();    
}

void Game::Face::Death()
{
    Game::Sound* sptr = ((Game::Sound*)this->associated.GetComponent("Sound"));
    if(sptr)
        sptr->Play();
    this->associated.RequestDelete();
}

void Game::Face::Update(float dt) {}

void Game::Face::Render() {}

///////////////////////////////////// Game TileSet Functions ////////////////////////////

Game::TileSet::TileSet(int tileWidth, int tileHeight, const std::string& file)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    
    this->tileSet.Open(file);
    this->rows = this->tileSet.GetHeight()/this->tileHeight;
    this->columns = this->tileSet.GetWidth()/this->tileWidth;
}

void Game::TileSet::RenderTile(unsigned index, float x, float y)
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

Game::TileMap::TileMap(Game::GameObject& associated, 
            const std::string& file, Game::TileSet* tileSet)
    : Component(associated)
{
    Load(file);
    SetTileSet(tileSet);
}
    
void Game::TileMap::Load(const std::string& file)
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

Game::Rect Game::TileMap::adapt(const Game::Rect& tSetC)
{
    const float tW = this->tileSet->GetTileWidth();
    const float tH = this->tileSet->GetTileHeight();
    
    const float xl = floor(tSetC.x/tW)*tW;
    const float yl = floor(tSetC.y/tH)*tH;

    const float wl = ceil((tSetC.x+tSetC.w)/tW)*tW;
    const float hl = ceil((tSetC.y+tSetC.h)/tH)*tH;
    
    return {xl,yl,wl,hl};
}

Game::Rect Game::TileMap::tileCord(Game::Rect& tSetC)
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

void Game::TileMap::Render()
{
    Game& inst = Game::GetInstance();

    int cameraX = 0, cameraY = 0;

    Game::Rect gCamera = { (float)cameraX, (float)cameraY, 
                           (float)inst.GetWidth(), (float)inst.GetHeight() };

    if(!this->associated.box.intersect(gCamera))
        return;
    
    Game::Rect toRender = adapt(gCamera.intersection(this->associated.box.copy()));
    Game::Rect tSetC = tileCord(toRender);
    
    for(int i = 0; i < this->mapDepth; i++)
        RenderLayer(i, toRender, tSetC);
}

void Game::TileMap::RenderLayer(int layer, const Game::Rect& toRender, const Game::Rect& tSetC)
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

SDL_Texture* Game::Resources::GetImage(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, SDL_Texture*>::iterator it = imageTable.find(file);
    
    if(it == imageTable.end())
        it = imageTable.insert({file, IMG_LoadTexture(renderer, file.c_str())}).first;
    
    return it->second;
}

Mix_Music* Game::Resources::GetMusic(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, Mix_Music*>::iterator it = musicTable.find(file);
    
    if(it == musicTable.end())
        it = musicTable.insert({file, Mix_LoadMUS(file.c_str())}).first;
    
    return it->second;
}

Mix_Chunk* Game::Resources::GetSound(const std::string& file)
{
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    std::unordered_map<std::string, Mix_Chunk*>::iterator it = soundTable.find(file);
    
    if(it == soundTable.end())
        it = soundTable.insert({file, Mix_LoadWAV(file.c_str())}).first;
    
    return it->second;
}

void Game::Resources::ClearImages() 
{ 
    for(std::unordered_map<std::string, SDL_Texture*>::iterator it = imageTable.begin();
                it != imageTable.end(); it++)
        SDL_DestroyTexture(it->second);

    imageTable.clear();
}

void Game::Resources::ClearMusics() 
{ 
    for(std::unordered_map<std::string, Mix_Music*>::iterator it = musicTable.begin();
                it != musicTable.end(); it++)
        Mix_FreeMusic(it->second);
        
    musicTable.clear();
}

void Game::Resources::ClearSounds() 
{ 
    for(std::unordered_map<std::string, Mix_Chunk*>::iterator it = soundTable.begin();
                it != soundTable.end(); it++)
        Mix_FreeChunk(it->second);
        
    soundTable.clear();
}