#include "Game.h"
#include "Zombie.h"
#include "Boss.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

/**
 * @file Game.cpp
 * @brief Реалізація головного класу гри Game.
 *
 * Цей файл містить всю логіку для керування станами гри (меню, гра, пауза),
 * ігровим циклом (обробка подій, оновлення, рендеринг), завантаження ресурсів
 * та відображення всіх графічних елементів за допомогою SFML.
 */

/**
 * @brief Конструктор класу Game.
 *
 * Ініціалізує посилання на вікно, початковий стан гри,
 * створює об'єкти гравця та карти.
 * Викликає методи для завантаження ресурсів та налаштування UI.
 *
 * @param win Посилання на головне вікно sf::RenderWindow, створене в main.cpp.
 */
Game::Game(sf::RenderWindow& win)
    : window(win),
      currentState(GameState::MainMenu),
      player("Player", 100, 20, 1, 1), // Ім'я, Здоров'я, Шкода, X, Y
      map(15, 15, 20) // Ширина, Висота, % стін
{
    loadAssets(); // Завантажуємо шрифти та текстури
    setupUI();    // Налаштовуємо кнопки та текст
}

/**
 * @brief Деструктор класу Game.
 * Використовує деструктор за замовчуванням.
 */
Game::~Game() = default;

/**
 * @brief Завантажує всі необхідні ассети (шрифти та текстури).
 *
 * Виводить повідомлення про помилку в std::cout, якщо
 * ресурс не може бути завантажений.
 * @note У релізній версії варто було б обробляти
 * ці помилки (наприклад, кидати виняток).
 */
void Game::loadAssets() {
    if (!font.loadFromFile("3Dumb.ttf")) { std::cout << "Error: Could not load font '3Dumb.ttf'" << std::endl; }
    if (!playerTexture.loadFromFile("player.png")) { std::cout << "Error loading player.png" << std::endl; }
    if (!zombieTexture.loadFromFile("zombie.png")) { std::cout << "Error loading zombie.png" << std::endl; }
    if (!bossTexture.loadFromFile("boss.png"))   { std::cout << "Error loading boss.png" << std::endl; }
    if (!wallTexture.loadFromFile("wall.png"))     { std::cout << "Error loading wall.png" << std::endl; }
    if (!floorTexture.loadFromFile("floor.png"))   { std::cout << "Error loading floor.png" << std::endl; }
}

/**
 * @brief Допоміжний приватний метод для центрування походження тексту.
 *
 * Це дозволяє легко позиціонувати текст відносно його центру (наприклад, на кнопках).
 * @param text Посилання на об'єкт sf::Text, який потрібно модифікувати.
 */
void Game::centerTextOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

/**
 * @brief Налаштовує всі елементи UI для всіх станів гри.
 *
 * Ініціалізує кнопки (sf::RectangleShape) та текст (sf::Text)
 * для головного меню, меню паузи та екрану завершення гри,
 * а також для ігрового HUD (здоров'я, очки).
 */
void Game::setupUI() {
    float centerX = window.getSize().x / 2.0f;
    float buttonWidth = 200.f;
    float buttonHeight = 50.f;
    sf::Color buttonColor(100, 100, 100);
    sf::Color textColor = sf::Color::White;
    unsigned int charSize = 24;
    unsigned int titleCharSize = 50;

    // --- Головне меню ---
    menuTitleText.setFont(font);
    menuTitleText.setString("Zombie Survival");
    menuTitleText.setCharacterSize(titleCharSize);
    menuTitleText.setFillColor(textColor);
    centerTextOrigin(menuTitleText);
    menuTitleText.setPosition(centerX, window.getSize().y / 4.0f);

    playButton.setSize({buttonWidth, buttonHeight});
    playButton.setFillColor(buttonColor);
    playButton.setOrigin(buttonWidth / 2.0f, buttonHeight / 2.0f);
    playButton.setPosition(centerX, window.getSize().y / 2.0f);

    playButtonText.setFont(font);
    playButtonText.setString("New Game");
    playButtonText.setCharacterSize(charSize);
    playButtonText.setFillColor(textColor);
    centerTextOrigin(playButtonText);
    playButtonText.setPosition(playButton.getPosition());

    exitButton = playButton; // Копіюємо налаштування кнопки
    exitButton.setPosition(centerX, playButton.getPosition().y + 70.f);
    exitButtonText = playButtonText; // Копіюємо налаштування тексту
    exitButtonText.setString("Exit");
    centerTextOrigin(exitButtonText);
    exitButtonText.setPosition(exitButton.getPosition());

    // --- Меню паузи ---
    pauseTitleText = menuTitleText;
    pauseTitleText.setString("Paused");
    centerTextOrigin(pauseTitleText);
    pauseTitleText.setPosition(centerX, window.getSize().y / 4.0f);

    resumeButton = playButton;
    resumeButton.setPosition(centerX, window.getSize().y / 2.0f);
    resumeButtonText = playButtonText;
    resumeButtonText.setString("Resume");
    centerTextOrigin(resumeButtonText);
    resumeButtonText.setPosition(resumeButton.getPosition());

    pauseExitButton = exitButton;
    pauseExitButton.setPosition(centerX, resumeButton.getPosition().y + 70.f);
    pauseExitButtonText = exitButtonText;
    pauseExitButtonText.setString("Main Menu");
    centerTextOrigin(pauseExitButtonText);
    pauseExitButtonText.setPosition(pauseExitButton.getPosition());

    // --- Екран кінця гри ---
    gameOverTitleText = menuTitleText;
    gameOverTitleText.setString("Game Over");
    centerTextOrigin(gameOverTitleText);
    gameOverTitleText.setPosition(centerX, window.getSize().y / 4.0f);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(charSize);
    finalScoreText.setFillColor(textColor);
    // Позиція finalScoreText встановлюється в renderGameOver()

    restartButton = playButton;
    restartButton.setPosition(centerX, window.getSize().y / 2.0f + 30.f);
    restartButtonText = playButtonText;
    restartButtonText.setString("Restart");
    centerTextOrigin(restartButtonText);
    restartButtonText.setPosition(restartButton.getPosition());

    gameOverExitButton = exitButton;
    gameOverExitButton.setPosition(centerX, restartButton.getPosition().y + 70.f);
    gameOverExitButtonText = pauseExitButtonText; // "Main Menu"
    gameOverExitButtonText.setPosition(gameOverExitButton.getPosition());

    // --- Ігровий HUD ---
    healthText.setFont(font);
    healthText.setCharacterSize(18);
    healthText.setFillColor(textColor);
    healthText.setPosition(10.f, 10.f);
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(textColor);
    scoreText.setPosition(10.f, 30.f);
}

/**
 * @brief Скидає стан гри до початкового.
 *
 * Викликається при натисканні "New Game" або "Restart".
 * Скидає гравця, очищує контейнер ворогів і додає нових,
 * скидає прапорець ходу та очищує лог.
 * Переводить гру у стан `Playing`.
 */
void Game::resetGame() {
    player.reset(1, 1);
    player.chooseWeapon(1); // Автоматично даємо гравцю Меч
    enemies.clear();

    // Додаємо ворогів у контейнер
    enemies.add(make_unique<Zombie>("Zombie 1", 50, 10, 5, 5));
    enemies.add(make_unique<Zombie>("Zombie 2", 60, 15, 10, 3));
    enemies.add(make_unique<Boss>("BOSS", 120, 20, 10, 7, 7)); // Ім'я, Здоров'я, Шкода, Лють, X, Y

    isPlayerTurn = true;
    logMessages.clear();
    addLogMessage("Game started!");
    currentState = GameState::Playing;
}

// --- Головний цикл та обробники ---

/**
 * @brief Запускає головний ігровий цикл.
 *
 * Цей цикл триває, доки вікно SFML відкрите.
 * На кожній ітерації викликає обробку подій, оновлення логіки та рендеринг.
 */
void Game::runGameLoop() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

/**
 * @brief Обробляє всі події SFML (закриття вікна, клавіші, миша).
 *
 * Використовує `switch (currentState)` для делегування обробки
 * подій відповідному приватному методу (наприклад, `processMainMenuEvents`).
 */
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Делегування залежно від стану гри
        switch (currentState) {
            case GameState::MainMenu: processMainMenuEvents(event); break;
            case GameState::Playing:  processPlayingEvents(event);  break;
            case GameState::Paused:   processPausedEvents(event);   break;
            case GameState::GameOver: processGameOverEvents(event); break;
        }
    }
}

/**
 * @brief Оновлює ігрову логіку.
 *
 * Наразі оновлення логіки потрібне лише у стані `Playing`
 * (наприклад, для ходу ворогів).
 */
void Game::update() {
    if (currentState == GameState::Playing) {
        updatePlaying();
    }
}

/**
 * @brief Головний метод рендерингу.
 *
 * Очищує вікно і викликає відповідний приватний метод
 * рендерингу (`renderMainMenu`, `renderPlaying` тощо)
 * залежно від поточного стану гри.
 */
void Game::render() {
    window.clear(sf::Color::Black);
    switch (currentState) {
        case GameState::MainMenu: renderMainMenu(); break;
        case GameState::Playing:  renderPlaying();  break;
        case GameState::Paused:   renderPaused();   break;
        case GameState::GameOver: renderGameOver(); break;
    }
    window.display();
}

// --- Обробники подій для станів ---

/**
 * @brief Обробляє події миші для Головного Меню.
 * @param event Подія SFML (наприклад, натискання кнопки миші).
 */
void Game::processMainMenuEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (playButton.getGlobalBounds().contains(mousePos)) {
            resetGame(); // Почати нову гру
        }
        if (exitButton.getGlobalBounds().contains(mousePos)) {
            window.close(); // Закрити програму
        }
    }
}

/**
 * @brief Обробляє події клавіатури для стану "Гра".
 *
 * Обробляє рух (W, A, S, D), атаку (F) та паузу (Escape).
 * Встановлює `isPlayerTurn = false` після будь-якої дії.
 * @param event Подія SFML (наприклад, натискання клавіші).
 */
void Game::processPlayingEvents(sf::Event& event) {
    if (isPlayerTurn && event.type == sf::Event::KeyPressed) {
        bool actionTaken = false;
        if (event.key.code == sf::Keyboard::W) { player.move(0, -1, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::S) { player.move(0, 1, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::A) { player.move(-1, 0, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::D) { player.move(1, 0, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::F) { handlePlayerAttack(); actionTaken = true; }
        if (event.key.code == sf::Keyboard::Escape) { currentState = GameState::Paused; }

        if (actionTaken && player.isAlive()) {
            isPlayerTurn = false; // Передаємо хід ворогам
        }
    }
}

/**
 * @brief Обробляє події миші та клавіатури для Меню Паузи.
 * @param event Подія SFML.
 */
void Game::processPausedEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (resumeButton.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::Playing; // Повернутись до гри
        }
        if (pauseExitButton.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::MainMenu; // Вийти в головне меню
        }
    }
    // Дозволяємо зняти паузу також клавішею Escape
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        currentState = GameState::Playing;
    }
}

/**
 * @brief Обробляє події миші для екрану "Гра завершена".
 * @param event Подія SFML.
 */
void Game::processGameOverEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (restartButton.getGlobalBounds().contains(mousePos)) {
            resetGame(); // Почати гру спочатку
        }
        if (gameOverExitButton.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::MainMenu; // Вийти в головне меню
        }
    }
}

// --- Оновлення логіки гри ---

/**
 * @brief Оновлює ігрову логіку у стані "Гра".
 *
 * Цей метод реалізує покрокову систему:
 * 1. Якщо хід не гравця (`!isPlayerTurn`), запускається ШІ ворогів.
 * 2. Вороги атакують гравця, якщо стоять поруч (dx + dy == 1).
 * 3. Перевіряється стан гравця (чи живий).
 * 4. Перевіряється, чи переможені всі вороги.
 * 5. Оновлюються тексти HUD (здоров'я, очки).
 */
void Game::updatePlaying() {
    // Хід Ворогів
    if (!isPlayerTurn && player.isAlive()) {
        addLogMessage("Enemy turn.");
        for (auto* e : enemies.getAllRaw()) {

            if (currentState == GameState::GameOver) break; // Якщо гравець загинув під час ітерації

            if (auto* z = dynamic_cast<Zombie*>(e)) {
                // Простий ШІ: якщо гравець поруч, атакувати
                int dx = abs(z->getX() - player.getX());
                int dy = abs(z->getY() - player.getY());
                if (dx + dy == 1) { // Ворог стоїть впритул до гравця
                    addLogMessage(z->getName() + " attacks Player.");
                    z->attack(player);
                    if (!player.isAlive()) {
                        addLogMessage("Player has fallen!");
                        currentState = GameState::GameOver;
                        gameOverTitleText.setString("Defeat..."); // Змінюємо заголовок
                        centerTextOrigin(gameOverTitleText);
                        break; // Завершуємо хід ворогів
                    }
                }
                // (Тут можна додати логіку руху ворогів, якщо вони не поруч)
            }
        }

        if (currentState != GameState::GameOver) {
             isPlayerTurn = true; // Повертаємо хід гравцю
        }
    }

    // Перевірка на перемогу
    if (currentState == GameState::Playing && enemies.size() == 0) {
        currentState = GameState::GameOver;
        gameOverTitleText.setString("Victory!"); // Змінюємо заголовок
        centerTextOrigin(gameOverTitleText);
        addLogMessage("All enemies defeated!");
    }

    // Оновлення HUD
    healthText.setString("Health: " + std::to_string(player.getHealth()));
    scoreText.setString("Score: " + std::to_string(player.getScore()));
}

// --- Методи рендерингу для станів ---

/**
 * @brief Малює елементи головного меню.
 */
void Game::renderMainMenu() {
    window.draw(menuTitleText);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

/**
 * @brief Малює ігровий світ (стан "Гра").
 *
 * Малює пошарово:
 * 1. Плитки карти (підлога, стіни).
 * 2. Ворогів.
 * 3. Гравця.
 * 4. HUD (здоров'я, очки).
 * 5. Ігровий лог.
 */
void Game::renderPlaying() {
    const int TILE_SIZE = 32; // Розмір тайлу 32x32 пікселі

    // 1. Малюємо карту
    for (int y = 0; y < 15; ++y) {
        for (int x = 0; x < 15; ++x) {
            sf::Sprite tileSprite;
            tileSprite.setTexture((map.getGrid()[y][x] == 1) ? wallTexture : floorTexture);
            tileSprite.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
            window.draw(tileSprite);
        }
    }

    // 2. Малюємо ворогів
    for (auto* e : enemies.getAllRaw()) {
        if (auto* z = dynamic_cast<Zombie*>(e)) {
            sf::Sprite enemySprite;
            // Використовуємо RTTI (dynamic_cast) щоб обрати правильну текстуру
            enemySprite.setTexture(dynamic_cast<Boss*>(z) ? bossTexture : zombieTexture);
            enemySprite.setPosition(static_cast<float>(z->getX() * TILE_SIZE), static_cast<float>(z->getY() * TILE_SIZE));
            window.draw(enemySprite);
        }
    }

    // 3. Малюємо гравця
    sf::Sprite playerSprite(playerTexture);
    playerSprite.setPosition(static_cast<float>(player.getX() * TILE_SIZE), static_cast<float>(player.getY() * TILE_SIZE));
    window.draw(playerSprite);

    // 4. Малюємо HUD
    window.draw(healthText);
    window.draw(scoreText);

    // 5. Малюємо ігровий лог (знизу вгору)
    float logY = window.getSize().y - 20.f; // Починаємо з низу вікна
    for (auto it = logMessages.rbegin(); it != logMessages.rend(); ++it) {
        it->setPosition(10.f, logY);
        window.draw(*it);
        logY -= 20.f; // Рухаємось вгору для наступного повідомлення
    }
}

/**
 * @brief Малює екран паузи.
 *
 * Малює ігровий світ (renderPlaying), потім накладає
 * напівпрозорий оверлей та елементи меню паузи.
 */
void Game::renderPaused() {
    // 1. Малюємо гру на фоні
    renderPlaying();

    // 2. Малюємо оверлей
    sf::RectangleShape overlay({(float)window.getSize().x, (float)window.getSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Чорний, 150/255 прозорості
    window.draw(overlay);

    // 3. Малюємо елементи меню паузи
    window.draw(pauseTitleText);
    window.draw(resumeButton);
    window.draw(resumeButtonText);
    window.draw(pauseExitButton);
    window.draw(pauseExitButtonText);
}

/**
 * @brief Малює екран "Гра завершена".
 *
 * Оновлює позицію тексту фінального рахунку та малює
 * всі елементи UI для цього стану.
 */
void Game::renderGameOver() {
    // Оновлюємо текст (рахунок міг змінитись) та позицію
    finalScoreText.setString("Final Score: " + std::to_string(player.getScore()));
    centerTextOrigin(finalScoreText);
    finalScoreText.setPosition(window.getSize().x / 2.0f, gameOverTitleText.getPosition().y + 70.f);

    window.draw(gameOverTitleText);
    window.draw(finalScoreText);
    window.draw(restartButton);
    window.draw(restartButtonText);
    window.draw(gameOverExitButton);
    window.draw(gameOverExitButtonText);
}

// --- Допоміжні методи логіки ---

/**
 * @brief Обробляє дію атаки гравця (клавіша F).
 *
 * Шукає ворога на сусідніх клітинках (dx + dy == 1).
 * Якщо ворога знайдено, гравець атакує його.
 * Якщо ворог гине, він видаляється з контейнера, і гравець отримує очки.
 */
void Game::handlePlayerAttack() {
    bool attacked = false;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (auto* z = dynamic_cast<Zombie*>(enemies.get(i))) {
            int dx = abs(z->getX() - player.getX());
            int dy = abs(z->getY() - player.getY());

            if (dx + dy == 1) { // Знайшли ворога поруч
                addLogMessage("Player attacks " + z->getName() + ".");
                player.attack(*z);
                attacked = true;

                if (!z->isAlive()) {
                    addLogMessage(z->getName() + " defeated!");
                    player.addScore(50); // Нагорода за вбивство
                    enemies.remove(i);   // Видаляємо ворога
                    i--; // Компенсуємо зсув вектора
                    break; // Гравець може атакувати лише одного ворога за хід
                }
                break; // Атакували, виходимо з циклу
            }
        }
    }
    if (!attacked) {
        addLogMessage("No enemy in range!");
    }
}

/**
 * @brief Додає повідомлення до ігрового логу на екрані.
 *
 * Якщо лог повний (досяг `MAX_LOG_MESSAGES`),
 * найстаріше повідомлення видаляється.
 * @param message Рядок, який потрібно додати в лог.
 */
void Game::addLogMessage(const std::string& message) {
    if (logMessages.size() >= MAX_LOG_MESSAGES) {
        logMessages.pop_front(); // Видаляємо найстаріше
    }
    sf::Text newLog;
    newLog.setFont(font);
    newLog.setString(message);
    newLog.setCharacterSize(14);
    newLog.setFillColor(sf::Color::White);
    logMessages.push_back(newLog); // Додаємо нове
}