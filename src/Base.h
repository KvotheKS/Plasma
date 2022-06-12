#pragma once

#include <./SDL2/SDL.h>
#include <./SDL2/SDL_image.h>
#include <./SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>

class Game
{

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

    public:
        Game::Vec2 sum(const Game::Vec2& vecl, const Game::Vec2& vecr);
        Game::Vec2 sub(const Game::Vec2& vecl, const Game::Vec2& vecr);
        Game::Vec2 mulc(const Game::Vec2& vec, const float scalar);
        Game::Vec2 divc(const Game::Vec2& vec, const float scalar);
        float mag(const Game::Vec2& vec);
        Game::Vec2 norm(const Game::Vec2& vec);
        float dist(const Game::Vec2& vecl, const Game::Vec2& vecr);
        float theta1(const Game::Vec2& vec);
        float theta(const Game::Vec2& vecl, const Game::Vec2& vecr);

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
    };

///////////////////////////////////////////// Game Music Class ///////////////////////////////////////////////

    class Music
    {
    private:
        Mix_Music* music;
    public:
        Music() { this->music = nullptr; }
        Music(const std::string& file);
        ~Music();
    public:
        void Play(int times = -1);
        void Stop(int msToStop=1500);
        void Open(const std::string& file);
    public:
        bool IsOpen() { return this->music != nullptr; }

    };
    
///////////////////////////////////////////// Game Sprite Class //////////////////////////////////////////////

    class Sprite
    {
    private:
        SDL_Texture* texture;
        int width, height;
        SDL_Rect clipRect;

    public:
        Sprite();

        Sprite(const std::string& file);

        ~Sprite();
    
    public:
        void Open(const std::string& file, int x, int y, int w=-1, int h=-1);

        void Open(const std::string& file);

        void SetClip(int x, int y, int w, int h);
    
    public:
        void Render(int x, int y);

    public:
        int GetWidth(){ return this->width; }
        int GetHeight(){ return this->height; }
        bool IsOpen(){ return this->texture != nullptr; }
    };

//////////////////////////////////////////// Game State Class /////////////////////////////////////////////////

    class State
    {
    private:
        Game::Sprite bg; 
        Game::Music music;
        bool quitRequested;
    public:
        State();

        bool QuitRequested() { return this->quitRequested; }
        
        void LoadAssets();
        
        void Update(float dt);

        void Render();
    };

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

    void test()
    {
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
        SDL_RenderClear(this->renderer);
        SDL_RenderPresent(this->renderer);
        SDL_Delay(3000);
    }
};

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
        std::cout << "ok";
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
}

//////////////////////////////////////////////// Game State Functions /////////////////////////////////////////////////

Game::State::State() 
{
    this->quitRequested = false;
    LoadAssets();
}

void Game::State::LoadAssets()
{
    this->bg.Open("./resources/img/ocean.jpg");
    this->music.Open("./resources/audio/stageState.ogg");
}

void Game::State::Update(float dt)
{
    if(SDL_QuitRequested())
        this->quitRequested = true;
}

void Game::State::Render()
{
    this->bg.Render(0,0);
    this->music.Play();
}

///////////////////////////////////////////////// Game Sprites ///////////////////////////////////////////////

Game::Sprite::Sprite()
{
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->clipRect = {0, 0, 0, 0};
}

Game::Sprite::Sprite(const std::string& file)
{
    Sprite();
    Open(file);
}

Game::Sprite::~Sprite()
{
    if(this->texture)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
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
    const auto renderer = Game::GetInstance().GetRenderer();

    if(this->texture)
        SDL_DestroyTexture(this->texture);
    this->texture = IMG_LoadTexture(renderer ,file.c_str());

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

void Game::Sprite::Render(int x, int y)
{
    const auto renderer = Game::GetInstance().GetRenderer();
    SDL_Rect dst = {
        x, y, this->clipRect.w, this->clipRect.h
    };

    SDL_RenderCopy(renderer, this->texture, &this->clipRect, &dst);
}

////////////////////////////////////////// Game Music Functions ////////////////////////////////////////////////

Game::Music::Music(const std::string& file)
{
    Open(file);
}

Game::Music::~Music()
{
    if(this->music)
    {
        Stop();
        Mix_FreeMusic(this->music);
    }
}

void Game::Music::Open(const std::string& file)
{
    this->music = Mix_LoadMUS(file.c_str());
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