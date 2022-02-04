#include "../include/classes.hpp"

using namespace std;

// THE GAME 1 //
Game theGame("Guns'n'Jets"); //Tytul

int main()
{
    srand(time(NULL)); setlocale(LC_ALL, "");
    Game::startConsole();
    theGame.run();
    return 0;
}
































