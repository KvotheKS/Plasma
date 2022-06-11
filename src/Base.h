#pragma once

#include <./SDL2/SDL.h>
#include <./SDL2/SDL_image.h>
#include <./SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <stdexcept>

class Game
{

public:
    
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