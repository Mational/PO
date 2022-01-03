#include "../include/classes.hpp"

using namespace std;

/// THE GAME 1 ///
Game theGame("Game 1"); /// globalna gra, The Gra

int main()
{
    srand(time(NULL)); setlocale(LC_ALL, "");
    Game::startConsole();
    theGame.run();
    return 0;
}
































