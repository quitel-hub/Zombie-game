#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Player.h"
#include "Map.h"
#include "Container.h"
#include "LocalizationManager.h"


class Command;
/**
 * @brief Головний клас гри, що керує ігровим циклом, станами та рендерингом.
 *
 * Цей клас використовує SFML для відображення графіки та UI.
 * Керує станами гри (меню, гра, пауза, кінець гри).
 */
class Game {
public:
    /**
     * @brief Перелік можливих станів гри.
     */
    enum class GameState { MainMenu, Playing, Paused, GameOver };

    /**
         * @brief Конструктор гри.
         * @param win Посилання на головне вікно SFML, в якому відбувається рендеринг.
         */
    Game(sf::RenderWindow& win);
    /**
     * @brief Деструктор.
     */
    ~Game();
    /**
         * @brief Запускає головний ігровий цикл.
         *
         * Цей метод містить `while (window.isOpen())`, який викликає
         * processEvents(), update() та render() залежно від стану гри.
         */
    void runGameLoop();

    /**
         * @brief Отримує посилання на об'єкт гравця.
         * @return Player& посилання на гравця.
         */
    Player& getPlayer() { return player; }
    /**
     * @brief Отримує посилання на об'єкт карти.
     * @return Map& посилання на карту.
     */
    Map& getMap() { return map; }
    /**
     * @brief Отримує посилання на контейнер з ворогами.
     * @return Container<Entity>& посилання на контейнер.
     */
    Container<Entity>& getEnemies() { return enemies; }

    /**
         * @brief Обробляє логіку атаки гравця.
         *
         * Викликається командою AttackCommand.
         * (Логіка, ймовірно, перевіряє ворогів поблизу та викликає Player::attack)
         */
    void handlePlayerAttack();

     /**
          * @brief Встановлює стан гри "Гра завершена".
          */
    void setGameOver() { currentState = GameState::GameOver; }

private:
    // --- Поля класу ---
    sf::RenderWindow& window; ///< Посилання на головне вікно (належить main.cpp)
    GameState currentState;   ///< Поточний стан гри (меню, гра тощо)

    Player player;             ///< Об'єкт гравця
    Container<Entity> enemies; ///< Контейнер для зберігання ворогів (Зомбі, Боси)
    Map map;                   ///< Об'єкт ігрової карти

    // --- Графічні ресурси ---
    sf::Font font;
    sf::Texture floorTexture, wallTexture, playerTexture, zombieTexture, bossTexture;

    // --- Елементи UI (Меню, Пауза, Кінець гри) ---
    sf::Text menuTitleText, playButtonText, exitButtonText;
    sf::RectangleShape playButton, exitButton;
    sf::Text pauseTitleText, resumeButtonText, pauseExitButtonText;
    sf::RectangleShape resumeButton, pauseExitButton;
    sf::Text gameOverTitleText, finalScoreText, restartButtonText, gameOverExitButtonText;
    sf::RectangleShape restartButton, gameOverExitButton;

    // --- Елементи UI (Ігровий процес) ---
    sf::Text healthText, scoreText;
    std::deque<sf::Text> logMessages; ///< Двостороння черга для ігрового логу
    const size_t MAX_LOG_MESSAGES = 4; ///< Макс. к-сть повідомлень в лозі

    bool isPlayerTurn = true; ///< Прапорець для покрокової логіки

    // --- Приватні методи ---

    /** @brief Завантажує всі ассети (шрифти, текстури). */
    void loadAssets();
    /** @brief Налаштовує всі текстові елементи та кнопки UI. */
    void setupUI();
    /** @brief Скидає гру до початкового стану. */
    void resetGame();
    /** @brief Додає нове повідомлення до ігрового логу на екрані. */
    void addLogMessage(const std::string& message);
    /** @brief Центрує походження (origin) тексту (для легкого позиціонування). */
    void centerTextOrigin(sf::Text& text);

    /** @brief Головний обробник подій (викликає специфічні для стану). */
    void processEvents();
    /** @brief Головний метод оновлення логіки (викликає специфічні для стану). */
    void update();
    /** @brief Головний метод рендерингу (викликає специфічні для стану). */
    void render();

    // --- Обробники подій для кожного стану ---
    void processMainMenuEvents(sf::Event& event);
    void processPlayingEvents(sf::Event& event);
    void processPausedEvents(sf::Event& event);
    void processGameOverEvents(sf::Event& event);

    /** @brief Оновлює логіку гри, коли стан Playing (наприклад, хід ворогів). */
    void updatePlaying();

    // --- Методи рендерингу для кожного стану ---
    void renderMainMenu();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();
};