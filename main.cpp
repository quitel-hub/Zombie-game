/**
 * @file main.cpp
 * @brief Точка входу в програму.
 * @details Ініціалізує вікно, налаштовує кодування та запускає головний цикл гри.
 */

#define NOMINMAX
#include <windows.h>

#include <SFML/Graphics.hpp>
#include <ctime>
#include "Game.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);


    srand(static_cast<unsigned int>(time(0)));


    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();


    sf::RenderWindow window(desktop, "Zombie Survival", sf::Style::Fullscreen);
    window.setFramerateLimit(60);


    Game game(window);


    game.runGameLoop();

    return 0;
}