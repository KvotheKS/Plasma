#include "./Base.h"

Game* Game::instance = nullptr;
bool Game::allocated = false;

int main(int argc, char** argv)
{
    Game ge = Game::GetInstance("Alexsander Correa de Oliveira - 190023562", 1024, 600);
    ge.Run();
}