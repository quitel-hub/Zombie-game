#include <SFML/Graphics.hpp>
#include "Game.h"
#include "LocalizationManager.h"

int main() {


    L10N.loadLanguage("ukr"); //


    sf::RenderWindow window(sf::VideoMode(480, 480), "Zombie Survival");
    window.setFramerateLimit(60);


    Game game(window);


    game.runGameLoop();

    return 0;
}