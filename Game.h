#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Player.h"
#include "Map.h"
#include "Container.h"
#include "LocalizationManager.h"

class Command;

class Game {
public:

    enum class GameState {
        MainMenu,
        ConfigSelection,
        Playing,
        Paused,
        GameOver
    };

private:
    sf::RenderWindow& window;
    GameState currentState;

    void centerTextOrigin(sf::Text& text);

    // --- Ігрові об'єкти ---
    Player player;
    Container<Entity> enemies;
    Map map;
    sf::View gameView;

    // --- Змінні конфігурації гри ---
    int configMapWidth;
    int configMapHeight;
    int configEnemyCount;
    float playerMaxHealth;

    // --- Графічні ресурси ---
    sf::Font font;
    sf::Texture floorTexture;
    sf::Texture wallTexture;
    sf::Texture playerTexture;
    sf::Texture zombieTexture;
    sf::Texture bossTexture;

    // --- Елементи інтерфейсу (UI) ---

    // Головне меню
    sf::Text menuTitleText;
    sf::RectangleShape playButton;
    sf::Text playButtonText;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    // Меню конфігурації
    sf::Text configTitleText;
    sf::Text mapWidthText;
    sf::RectangleShape mapWidthIncreaseButton;
    sf::Text mapWidthIncreaseText;
    sf::RectangleShape mapWidthDecreaseButton;
    sf::Text mapWidthDecreaseText;
    sf::Text mapHeightText;
    sf::RectangleShape mapHeightIncreaseButton;
    sf::Text mapHeightIncreaseText;
    sf::RectangleShape mapHeightDecreaseButton;
    sf::Text mapHeightDecreaseText;
    sf::Text enemyCountText;
    sf::RectangleShape enemyCountIncreaseButton;
    sf::Text enemyCountIncreaseText;
    sf::RectangleShape enemyCountDecreaseButton;
    sf::Text enemyCountDecreaseText;
    sf::RectangleShape configStartButton;
    sf::Text configStartButtonText;
    sf::RectangleShape configBackButton;
    sf::Text configBackButtonText;

    // Меню паузи
    sf::Text pauseTitleText;
    sf::RectangleShape resumeButton;
    sf::Text resumeButtonText;
    sf::RectangleShape pauseRestartButton;
    sf::Text pauseRestartButtonText;
    sf::RectangleShape pauseToMenuButton;
    sf::Text pauseToMenuButtonText;
    sf::RectangleShape pauseExitDesktopButton;
    sf::Text pauseExitDesktopButtonText;

    // Екран кінця гри (Game Over)
    sf::Text gameOverTitleText;
    sf::Text finalScoreText;
    sf::RectangleShape restartButton;
    sf::Text restartButtonText;
    sf::RectangleShape gameOverExitButton;
    sf::Text gameOverExitButtonText;

    // Ігровий HUD та лог
    sf::Text healthText;
    sf::Text scoreText;
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    std::deque<sf::Text> logMessages;
    const size_t MAX_LOG_MESSAGES = 4;

    bool isPlayerTurn = true;


    // --- Приватні методи ---

    void loadAssets();
    void setupUI();
    void processEvents();
    void update();
    void render();
    void processMainMenuEvents(sf::Event& event);
    void processConfigSelectionEvents(sf::Event& event);
    void processPlayingEvents(sf::Event& event);
    void processPausedEvents(sf::Event& event);
    void processGameOverEvents(sf::Event& event);
    void updatePlaying();
    void renderMainMenu();
    void renderConfigSelection();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();
    void handlePlayerAttack();
    void addLogMessage(const std::string& message);
    void resetGame();

public:
    Game(sf::RenderWindow& win);
    ~Game();

    void runGameLoop();

    Player& getPlayer() { return player; }
    Map& getMap() { return map; }
    Container<Entity>& getEnemies() { return enemies; }
};