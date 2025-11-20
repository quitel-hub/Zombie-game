#define NOMINMAX
#include <windows.h>


#include <SFML/Graphics.hpp>
#include <ctime>
#include "Game.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(480, 480), "Zombie Game");
    window.setFramerateLimit(60);


    Game game(window);


    game.runGameLoop();

    return 0;
}