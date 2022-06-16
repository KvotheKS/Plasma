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
        Game::Vec2 sum(const Game::Vec2& vecl, const Game::Vec2& vecr);
        Game::Vec2 sub(const Game::Vec2& vecl, const Game::Vec2& vecr);
        Game::Vec2 mulc(const Game::Vec2& vec, const float scalar);
        Game::Vec2 divc(const Game::Vec2& vec, const float scalar);
        float mag(const Game::Vec2& vec);
        Game::Vec2 norm(const Game::Vec2& vec);
        float dist(const Game::Vec2& vecl, const Game::Vec2& vecr);
        float theta1(const Game::Vec2& vec);
        float theta(const Game::Vec2& vecl, const Game::Vec2& vecr);
        Game::Vec2& Rotate(Game::Vec2 vec,float angle)
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

    class Sound : public Component
    {
    private:
        Mix_Chunk* chunk;
        int channel;

    public:
        Sound() : Component() {}
        Sound(Game::GameObject& associated) : Component(associated) 
        { this->chunk = nullptr; this->channel = -1; }
        Sound(Game::GameObject& associated, const std::string& file);
        ~Sound() { Mix_FreeChunk(this->chunk); this->chunk = nullptr; }
    
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
        Music(Game::GameObject& associated, const std::string& file);
        ~Music();
    public:
        void Play(int times = -1);
        void Stop(int msToStop=1500);
        void Open(const std::string& file);
    public:
        
        void Update(float dt) {}
        void Render() {}
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

    class Face : public Component
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

    void test()
    {
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
        SDL_RenderClear(this->renderer);
        SDL_RenderPresent(this->renderer);
        SDL_Delay(3000);
    }
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
        this->channel = Mix_PlayChannel(-1, this->chunk, times);
    
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
    if(this->chunk)
        Mix_FreeChunk(this->chunk);
    
    this->chunk = Mix_LoadWAV(file.c_str());
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
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    
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


///////////////////////////////////////// Game GameObject Functions /////////////////////////////////////////


Game::GameObject::GameObject()
{
    this->isDead = false;
}

Game::GameObject::~GameObject()
{    
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
        msc
    });

    this->bg.box = { 0, 0, (float)inst.GetWidth(), (float)inst.GetHeight() };
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
    ((Game::Music*)this->bg.GetComponent("Music"))->Play();
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
                std::cout << mouseX << '|' << mouseY << '\n';
				Game::Vec2 objPos = Game::Vec2( 200, 0 ).Rotate( -M_PI + M_PI*(rand() % 1001)/500.0 ) + Game::Vec2( mouseX, mouseY );
				AddObject((int)objPos.x, (int)objPos.y);
			}
		}
	}
}

//////////////// to-do: box, unique_ptr em GameObject.
void Game::State::AddObject(int mouseX, int mouseY)
{
    this->objectArray.push_back(std::unique_ptr<Game::GameObject>(new Game::GameObject()));
    const int vSize = this->objectArray.size() - 1;
    
    Game::Sprite* spr = new Sprite(*this->objectArray[vSize], "resources/img/penguinface.png");
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