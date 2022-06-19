#include "./Base.h"

Game* Game::instance = nullptr;
bool Game::allocated = false;
std::unordered_map<std::string, SDL_Texture*> Game::Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Game::Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Game::Resources::soundTable;

int main(int argc, char** argv)
{
    Game& ge = Game::GetInstance("Alexsander Correa de Oliveira - 190023562", 1024, 600);
    ge.Run();
}