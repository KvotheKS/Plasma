#include "./Base.h"

Game* Game::instance = nullptr;
bool Game::allocated = false;
std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
Vec2 Camera::pos = {0,0}, Camera::speed = {10,10};
GameObject* Camera::focus = nullptr;

int main(int argc, char** argv)
{
    Game& ge = Game::GetInstance("Alexsander Correa de Oliveira - 190023562", 1024, 600);
    ge.Run();
}