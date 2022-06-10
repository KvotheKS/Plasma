#include <./SDL2/SDL.h>
#include <./SDL2/SDL_image.h>
#include <./SDL2/SDL_mixer.h>
#include <iostream>
#include <stdexcept>

class Game
{

public:
    class Music;
    
    class Sprite;

    class State
    {
    private:
        //Sprite bg; 
        //Game::Music music;
        bool quitRequested = false;
    public:
        State();
        void QuitRequested();
        void LoadAssets();
        void Update(float dt);
        void Render();
    };

private:
    static Game* instance;
    static bool allocated;
public:
    
    class __initializer__
    {
    public:
        __initializer__()
        {
            std::cout << "INITIALIZED";
            Game::instance = nullptr;
            Game::allocated = false;
        }
    };

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    State* state = nullptr;

private:
    static void ThrowException()
    {
        throw std::runtime_error(SDL_GetError());
        abort();
    }

private:
    Game(const char* Title = "Titulo", int width = 600, int height = 480)
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
    }

public:
    ~Game()
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

    void Run();
    
    SDL_Renderer* GetRenderer(){ return this->renderer; }
    
    State& GetState(){ return *this->state; }
    
    static Game& GetInstance(const char* Title = "Titulo", int width = 600, int height = 480)
    {  
        if(!Game::instance)
        {
            std::cout << "ok";
            Game::allocated = true;
            Game::instance = new Game(Title, width, height);
        }
        
        return *Game::instance;
    }

    void test()
    {
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
        SDL_RenderClear(this->renderer);
        SDL_RenderPresent(this->renderer);
        SDL_Delay(3000);
    }
};

