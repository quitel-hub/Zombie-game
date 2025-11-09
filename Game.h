/**
 * @file Game.h
 * @brief Визначення головного класу гри Game, який керує всіма ігровими станами та логікою.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Player.h"
#include "Map.h"
#include "Container.h"
#include "LocalizationManager.h"

class Command;

/**
 * @class Game
 * @brief Головний клас гри, що реалізує FSM (Finite State Machine) для керування станами гри.
 *
 * Цей клас відповідає за ініціалізацію, головний цикл, обробку подій,
 * оновлення логіки та рендеринг всіх ігрових об'єктів та інтерфейсу.
 */
class Game {
public:
    /**
     * @enum GameState
     * @brief Перелік можливих станів, у яких може перебувати гра.
     */
    enum class GameState {
        MainMenu,       ///< Стан головного меню.
        ConfigSelection, ///< Стан екрану налаштування нової гри.
        Playing,        ///< Стан активної гри.
        Paused,         ///< Стан паузи.
        GameOver        ///< Стан завершення гри (перемога або поразка).
    };

private:
    sf::RenderWindow& window;   ///< Посилання на головне вікно SFML.
    GameState currentState;     ///< Поточний стан гри (з GameState).

    /**
     * @brief Допоміжна функція для центрування походження (origin) тексту.
     * @param text Об'єкт sf::Text, який потрібно центрувати.
     */
    void centerTextOrigin(sf::Text& text);

    // --- Ігрові об'єкти ---
    Player player;              ///< Об'єкт гравця.
    Container<Entity> enemies;  ///< Контейнер для всіх ворогів (Зомбі, Боси).
    Map map;                    ///< Об'єкт ігрової карти.
    sf::View gameView;          ///< Ігрова камера (View), що слідує за гравцем.

    // --- Змінні конфігурації гри ---
    int configMapWidth;     ///< Ширина карти, обрана в меню конфігурації.
    int configMapHeight;    ///< Висота карти, обрана в меню конфігурації.
    int configEnemyCount;   ///< Кількість ворогів, обрана в меню конфігурації.
    float playerMaxHealth;  ///< Максимальне здоров'я гравця (для розрахунку HP бару).

    // --- Графічні ресурси ---
    sf::Font font;              ///< Основний шрифт гри.
    sf::Texture floorTexture;   ///< Текстура підлоги.
    sf::Texture wallTexture;    ///< Текстура стіни.
    sf::Texture playerTexture;  ///< Текстура гравця.
    sf::Texture zombieTexture;  ///< Текстура зомбі.
    sf::Texture bossTexture;    ///< Текстура боса.

    // --- Елементи інтерфейсу (UI) ---

    // Головне меню
    sf::Text menuTitleText;         ///< Текст заголовку.
    sf::RectangleShape playButton;  ///< Кнопка "New Game".
    sf::Text playButtonText;        ///< Текст кнопки "New Game".
    sf::RectangleShape exitButton;  ///< Кнопка "Exit".
    sf::Text exitButtonText;        ///< Текст кнопки "Exit".

    // Меню конфігурації
    sf::Text configTitleText;                   ///< Заголовок меню налаштувань.
    sf::Text mapWidthText;                      ///< Текст для "Map Width:".
    sf::RectangleShape mapWidthIncreaseButton;  ///< Кнопка '+'.
    sf::Text mapWidthIncreaseText;              ///< Текст '+'.
    sf::RectangleShape mapWidthDecreaseButton;  ///< Кнопка '-'.
    sf::Text mapWidthDecreaseText;              ///< Текст '-'.
    sf::Text mapHeightText;                     ///< Текст для "Map Height:".
    sf::RectangleShape mapHeightIncreaseButton; ///< Кнопка '+'.
    sf::Text mapHeightIncreaseText;             ///< Текст '+'.
    sf::RectangleShape mapHeightDecreaseButton; ///< Кнопка '-'.
    sf::Text mapHeightDecreaseText;             ///< Текст '-'.
    sf::Text enemyCountText;                    ///< Текст для "Enemies:".
    sf::RectangleShape enemyCountIncreaseButton;///< Кнопка '+'.
    sf::Text enemyCountIncreaseText;            ///< Текст '+'.
    sf::RectangleShape enemyCountDecreaseButton;///< Кнопка '-'.
    sf::Text enemyCountDecreaseText;            ///< Текст '-'.
    sf::RectangleShape configStartButton;       ///< Кнопка "Start Game".
    sf::Text configStartButtonText;             ///< Текст "Start Game".
    sf::RectangleShape configBackButton;        ///< Кнопка "Back to Menu".
    sf::Text configBackButtonText;              ///< Текст "Back to Menu".

    // Меню паузи
    sf::Text pauseTitleText;                    ///< Заголовок "Paused".
    sf::RectangleShape resumeButton;            ///< Кнопка "Resume".
    sf::Text resumeButtonText;                  ///< Текст "Resume".
    sf::RectangleShape pauseRestartButton;      ///< Кнопка "Restart" у меню паузи.
    sf::Text pauseRestartButtonText;            ///< Текст "Restart".
    sf::RectangleShape pauseToMenuButton;       ///< Кнопка "Main Menu" у меню паузи.
    sf::Text pauseToMenuButtonText;             ///< Текст "Main Menu".
    sf::RectangleShape pauseExitDesktopButton;  ///< Кнопка "Exit Game" у меню паузи.
    sf::Text pauseExitDesktopButtonText;        ///< Текст "Exit Game".

    // Екран кінця гри (Game Over)
    sf::Text gameOverTitleText;     ///< Заголовок "Victory!" або "Defeat...".
    sf::Text finalScoreText;        ///< Фінальний рахунок гравця.
    sf::RectangleShape restartButton;   ///< Кнопка "Restart" на екрані Game Over.
    sf::Text restartButtonText;         ///< Текст "Restart".
    sf::RectangleShape gameOverExitButton; ///< Кнопка "Main Menu" на екрані Game Over.
    sf::Text gameOverExitButtonText;       ///< Текст "Main Menu".

    // Ігровий HUD та лог
    sf::Text healthText;                        ///< Текст "Health: X".
    sf::Text scoreText;                         ///< Текст "Score: X".
    sf::RectangleShape healthBarBackground;     ///< Фон для HP бару.
    sf::RectangleShape healthBarForeground;     ///< Смуга HP бару (змінює ширину).
    std::deque<sf::Text> logMessages;           ///< Двостороння черга для повідомлень ігрового логу.
    const size_t MAX_LOG_MESSAGES = 4;          ///< Макс. кількість повідомлень у лозі.

    bool isPlayerTurn = true;   ///< Прапорець, що вказує, чи зараз хід гравця.


    // --- Приватні методи ---

    /**
     * @brief Завантажує всі необхідні ресурси (шрифти, текстури).
     */
    void loadAssets();

    /**
     * @brief Налаштовує всі елементи UI (кнопки, текст) при старті гри.
     */
    void setupUI();

    /**
     * @brief Обробляє всі події SFML (закриття вікна, натискання клавіш/миші).
     * Викликає відповідний обробник залежно від currentState.
     */
    void processEvents();

    /**
     * @brief Оновлює логіку гри. Викликає updatePlaying(), якщо стан Playing.
     */
    void update();

    /**
     * @brief Очищує вікно та викликає відповідний метод рендерингу залежно від currentState.
     */
    void render();

    /**
     * @brief Обробляє події для стану GameState::MainMenu.
     * @param event Подія SFML.
     */
    void processMainMenuEvents(sf::Event& event);

    /**
     * @brief Обробляє події для стану GameState::ConfigSelection.
     * @param event Подія SFML.
     */
    void processConfigSelectionEvents(sf::Event& event);

    /**
     * @brief Обробляє події для стану GameState::Playing (рух, атака, пауза).
     * @param event Подія SFML.
     */
    void processPlayingEvents(sf::Event& event);

    /**
     * @brief Обробляє події для стану GameState::Paused.
     * @param event Подія SFML.
     */
    void processPausedEvents(sf::Event& event);

    /**
     * @brief Обробляє події для стану GameState::GameOver.
     * @param event Подія SFML.
     */
    void processGameOverEvents(sf::Event& event);

    /**
     * @brief Оновлює ігрову логіку (хід ворогів, перевірка перемоги/поразки, оновлення камери).
     */
    void updatePlaying();

    /**
     * @brief Рендерить екран головного меню.
     */
    void renderMainMenu();

    /**
     * @brief Рендерить екран конфігурації гри.
     */
    void renderConfigSelection();

    /**
     * @brief Рендерить ігровий світ (карту, гравця, ворогів) та HUD.
     * Керує перемиканням між gameView (ігровий світ) та defaultView (HUD).
     */
    void renderPlaying();

    /**
     * @brief Рендерить екран паузи (ігровий світ + затемнення + меню паузи).
     */
    void renderPaused();

    /**
     * @brief Рендерить екран завершення гри.
     */
    void renderGameOver();

    /**
     * @brief Обробляє логіку атаки гравця.
     */
    void handlePlayerAttack();

    /**
     * @brief Додає нове повідомлення до ігрового логу.
     * @param message Текст повідомлення.
     */
    void addLogMessage(const std::string& message);

    /**
     * @brief Скидає стан гри до початкового, використовуючи обрані налаштування (configMapWidth тощо).
     */
    void resetGame();

public:
    /**
     * @brief Конструктор класу Game.
     * @param win Посилання на вікно SFML, в якому буде відбуватися рендеринг.
     */
    Game(sf::RenderWindow& win);

    /**
     * @brief Деструктор класу Game.
     */
    ~Game();

    /**
     * @brief Запускає головний ігровий цикл (while window.isOpen()).
     */
    void runGameLoop();

    /**
     * @brief Отримує посилання на об'єкт гравця.
     * @return Посилання на Player.
     */
    Player& getPlayer() { return player; }

    /**
     * @brief Отримує посилання на об'єкт карти.
     * @return Посилання на Map.
     */
    Map& getMap() { return map; }

    /**
     * @brief Отримує посилання на контейнер з ворогами.
     * @return Посилання на Container<Entity>.
     */
    Container<Entity>& getEnemies() { return enemies; }
};