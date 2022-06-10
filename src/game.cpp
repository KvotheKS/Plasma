#include "./class.cpp"

Game* Game::instance = nullptr;
bool Game::allocated = false;
// Inicializacao de membros static private:

int main()
{
    Game ge = Game::GetInstance();
    ge.test();
}