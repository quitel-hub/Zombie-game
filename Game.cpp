/**
 * @file Game.cpp
 * @brief Реалізація основної логіки гри.
 * @details Містить головний цикл, обробку подій, оновлення стану гри (Update)
 * та відображення (Render). Також відповідає за управління ресурсами (звуки, текстури).
 */
#include "Game.h"
#include "Zombie.h"
#include "Boss.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <cmath>

// Конструктор
/**
 * @brief Конструктор гри.
 * @details Ініціалізує вікно, гравця, карту та налаштування камери.
 * Також запускає завантаження ресурсів.
 */
Game::Game(sf::RenderWindow& win)
        : window(win),
          currentState(GameState::MainMenu),
          player("Player", 100, 20, 1, 1),
          map(15, 15, 20),
          playerMaxHealth(100.0f),
          configMapWidth(15),
          configMapHeight(15),
          configEnemyCount(3)
{

    gameView.setSize(800.f, 600.f);
    gameView.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

    LOG_INFO("Game engine initialized. Window size: " +
             to_string(window.getSize().x) + "x" + to_string(window.getSize().y));

    loadAssets();
    setupUI();
}

Game::~Game() {
    LOG_INFO("Game session ended. Shutting down.");
}

void Game::loadAssets() {
    LOG_INFO("Loading assets...");
    bool errorOccurred = false;

    if (!font.loadFromFile("DS Stamper.ttf") && !font.loadFromFile("assets/DS Stamper.ttf")) {
        LOG_ERR("CRITICAL: Could not load font '3Dumb.ttf'");
        errorOccurred = true;
    }
    if (!playerTexture.loadFromFile("player.png") && !playerTexture.loadFromFile("assets/player.png")) {
        LOG_ERR("Error loading player.png");
        errorOccurred = true;
    }
    if (!zombieTexture.loadFromFile("zombie.png") && !zombieTexture.loadFromFile("assets/zombie.png")) {
        LOG_ERR("Error loading zombie.png");
        errorOccurred = true;
    }
    if (!bossTexture.loadFromFile("boss.png") && !bossTexture.loadFromFile("assets/boss.png"))   {
        LOG_ERR("Error loading boss.png");
        errorOccurred = true;
    }
    if (!wallTexture.loadFromFile("wall.png") && !wallTexture.loadFromFile("assets/wall.png"))     {
        LOG_ERR("Error loading wall.png");
        errorOccurred = true;
    }
    if (!floorTexture.loadFromFile("floor.png") && !floorTexture.loadFromFile("assets/floor.png"))   {
        LOG_ERR("Error loading floor.png");
        errorOccurred = true;
    }

    if (!shootBuffer.loadFromFile("shoot.wav") && !shootBuffer.loadFromFile("assets/shoot.wav")) {
        LOG_ERR("Failed to load shoot.wav");
    }
    if (!hitBuffer.loadFromFile("hit.wav") && !hitBuffer.loadFromFile("assets/hit.wav")) {
        LOG_ERR("Failed to load hit.wav");
    }
    if (!pickupBuffer.loadFromFile("pickup.wav") && !pickupBuffer.loadFromFile("assets/pickup.wav")) {
        LOG_ERR("Failed to load pickup.wav");
    }
    if (!zombieBuffer.loadFromFile("zombie.wav") && !zombieBuffer.loadFromFile("assets/zombie.wav")) {
        LOG_ERR("Failed to load zombie.wav");
    }


    shootSound.setBuffer(shootBuffer);
    hitSound.setBuffer(hitBuffer);
    pickupSound.setBuffer(pickupBuffer);
    zombieSound.setBuffer(zombieBuffer);


    shootSound.setVolume(50);

    if (!bgMusic.openFromFile("music.ogg") && !bgMusic.openFromFile("assets/music.ogg")) {
        LOG_ERR("Failed to open music.ogg");
    } else {
        bgMusic.setLoop(true);
        bgMusic.setVolume(30);
        bgMusic.play();
        LOG_INFO("Background music started.");
    }


    if (!errorOccurred) {
        LOG_INFO("All assets loaded successfully.");
    } else {
        LOG_WARN("Some assets failed to load. Game may look incorrect.");
    }


}
// --- НАЛАШТУВАННЯ ІНТЕРФЕЙСУ ---
void Game::setupUI() {
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f; // Центр по Y

    float buttonWidth = 200.f;
    float buttonHeight = 50.f;
    sf::Color buttonColor(100, 100, 100);
    sf::Color textColor = sf::Color::Green;
    unsigned int charSize = 24;
    unsigned int titleCharSize = 50;

    // --- Головне меню (Центруємо відносно екрану) ---
    menuTitleText.setFont(font);
    menuTitleText.setString("Zombie Survival");
    menuTitleText.setCharacterSize(titleCharSize);
    menuTitleText.setFillColor(sf::Color::Green);
    centerTextOrigin(menuTitleText);
    menuTitleText.setPosition(centerX, centerY - 150.f);

    playButton.setSize({buttonWidth, buttonHeight});
    playButton.setFillColor(buttonColor);
    playButton.setOrigin(buttonWidth / 2.0f, buttonHeight / 2.0f);
    playButton.setPosition(centerX, centerY);

    playButtonText.setFont(font);
    playButtonText.setString("New Game");
    playButtonText.setCharacterSize(charSize);
    playButtonText.setFillColor(textColor);
    centerTextOrigin(playButtonText);
    playButtonText.setPosition(playButton.getPosition());

    exitButton = playButton;
    exitButton.setPosition(centerX, playButton.getPosition().y + 70.f);
    exitButtonText = playButtonText;
    exitButtonText.setString("Exit");
    centerTextOrigin(exitButtonText);
    exitButtonText.setPosition(exitButton.getPosition());

    // --- Меню конфігурації ---
    configTitleText = menuTitleText;
    configTitleText.setString("Game Configuration");
    configTitleText.setCharacterSize(40);
    centerTextOrigin(configTitleText);
    configTitleText.setPosition(centerX, centerY - 200.f);

    float buttonSize = 40.f;
    float labelY_MapWidth = centerY - 50.f;
    float labelY_MapHeight = labelY_MapWidth + 80.f;
    float labelY_Enemies = labelY_MapHeight + 80.f;
    sf::Color smallButtonColor(70, 70, 70);

    // Config UI
    mapWidthText.setFont(font);
    mapWidthText.setCharacterSize(charSize);
    mapWidthText.setFillColor(textColor);
    mapWidthText.setPosition(centerX, labelY_MapWidth);
    mapWidthDecreaseButton.setSize({buttonSize, buttonSize});
    mapWidthDecreaseButton.setFillColor(smallButtonColor);
    mapWidthDecreaseButton.setOrigin(buttonSize / 2.0f, buttonSize / 2.0f);
    mapWidthDecreaseButton.setPosition(centerX - 150.f, labelY_MapWidth);
    mapWidthDecreaseText.setFont(font);
    mapWidthDecreaseText.setString("-");
    mapWidthDecreaseText.setCharacterSize(charSize);
    mapWidthDecreaseText.setFillColor(textColor);
    centerTextOrigin(mapWidthDecreaseText);
    mapWidthDecreaseText.setPosition(mapWidthDecreaseButton.getPosition());
    mapWidthIncreaseButton = mapWidthDecreaseButton;
    mapWidthIncreaseButton.setPosition(centerX + 150.f, labelY_MapWidth);
    mapWidthIncreaseText = mapWidthDecreaseText;
    mapWidthIncreaseText.setString("+");
    centerTextOrigin(mapWidthIncreaseText);
    mapWidthIncreaseText.setPosition(mapWidthIncreaseButton.getPosition());

    mapHeightText = mapWidthText;
    mapHeightText.setPosition(centerX, labelY_MapHeight);
    mapHeightDecreaseButton = mapWidthDecreaseButton;
    mapHeightDecreaseButton.setPosition(centerX - 150.f, labelY_MapHeight);
    mapHeightDecreaseText = mapWidthDecreaseText;
    mapHeightDecreaseText.setPosition(mapHeightDecreaseButton.getPosition());
    mapHeightIncreaseButton = mapWidthIncreaseButton;
    mapHeightIncreaseButton.setPosition(centerX + 150.f, labelY_MapHeight);
    mapHeightIncreaseText = mapWidthIncreaseText;
    mapHeightIncreaseText.setPosition(mapHeightIncreaseButton.getPosition());

    enemyCountText = mapWidthText;
    enemyCountText.setPosition(centerX, labelY_Enemies);
    enemyCountDecreaseButton = mapWidthDecreaseButton;
    enemyCountDecreaseButton.setPosition(centerX - 150.f, labelY_Enemies);
    enemyCountDecreaseText = mapWidthDecreaseText;
    enemyCountDecreaseText.setPosition(enemyCountDecreaseButton.getPosition());
    enemyCountIncreaseButton = mapWidthIncreaseButton;
    enemyCountIncreaseButton.setPosition(centerX + 150.f, labelY_Enemies);
    enemyCountIncreaseText = mapWidthIncreaseText;
    enemyCountIncreaseText.setPosition(enemyCountIncreaseButton.getPosition());

    configStartButton = playButton;
    configStartButton.setPosition(centerX, centerY + 150.f);
    configStartButtonText = playButtonText;
    configStartButtonText.setString("Start Game");
    centerTextOrigin(configStartButtonText);
    configStartButtonText.setPosition(configStartButton.getPosition());
    configBackButton = exitButton;
    configBackButton.setPosition(centerX, configStartButton.getPosition().y + 70.f);
    configBackButtonText = exitButtonText;
    configBackButtonText.setString("Back to Menu");
    centerTextOrigin(configBackButtonText);
    configBackButtonText.setPosition(configBackButton.getPosition());

    // Меню паузи
    pauseTitleText = menuTitleText;
    pauseTitleText.setString("Paused");
    centerTextOrigin(pauseTitleText);
    pauseTitleText.setPosition(centerX, centerY - 100.f);

    resumeButton = playButton;
    resumeButton.setPosition(centerX, centerY);
    resumeButtonText = playButtonText;
    resumeButtonText.setString("Resume");
    centerTextOrigin(resumeButtonText);
    resumeButtonText.setPosition(resumeButton.getPosition());

    pauseRestartButton = playButton;
    pauseRestartButton.setPosition(centerX, resumeButton.getPosition().y + 70.f);
    pauseRestartButtonText = playButtonText;
    pauseRestartButtonText.setString("Restart");
    centerTextOrigin(pauseRestartButtonText);
    pauseRestartButtonText.setPosition(pauseRestartButton.getPosition());

    pauseToMenuButton = exitButton;
    pauseToMenuButton.setPosition(centerX, pauseRestartButton.getPosition().y + 70.f);
    pauseToMenuButtonText = exitButtonText;
    pauseToMenuButtonText.setString("Main Menu");
    centerTextOrigin(pauseToMenuButtonText);
    pauseToMenuButtonText.setPosition(pauseToMenuButton.getPosition());

    pauseExitDesktopButton = exitButton;
    pauseExitDesktopButton.setPosition(centerX, pauseToMenuButton.getPosition().y + 70.f);
    pauseExitDesktopButtonText = exitButtonText;
    pauseExitDesktopButtonText.setString("Exit Game");
    centerTextOrigin(pauseExitDesktopButtonText);
    pauseExitDesktopButtonText.setPosition(pauseExitDesktopButton.getPosition());

    //Екран кінця гри
    gameOverTitleText = menuTitleText;
    gameOverTitleText.setString("Game Over");
    centerTextOrigin(gameOverTitleText);
    gameOverTitleText.setPosition(centerX, centerY - 100.f);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(charSize);
    finalScoreText.setFillColor(textColor);

    restartButton = playButton;
    restartButton.setPosition(centerX, centerY + 50.f);
    restartButtonText = playButtonText;
    restartButtonText.setString("Restart");
    centerTextOrigin(restartButtonText);
    restartButtonText.setPosition(restartButton.getPosition());

    gameOverExitButton = exitButton;
    gameOverExitButton.setPosition(centerX, restartButton.getPosition().y + 70.f);
    gameOverExitButtonText = pauseToMenuButtonText;
    gameOverExitButtonText.setPosition(gameOverExitButton.getPosition());

    //Ігровий HUD
    healthText.setFont(font);
    healthText.setCharacterSize(18);
    healthText.setFillColor(textColor);
    healthText.setPosition(10.f, 10.f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(textColor);
    scoreText.setPosition(10.f, 30.f);

    ammoText.setFont(font);
    ammoText.setCharacterSize(18);
    ammoText.setFillColor(sf::Color::Yellow);
    ammoText.setPosition(10.f, 80.f);

    healthBarBackground.setSize({100.f, 15.f});
    healthBarBackground.setFillColor(sf::Color(50, 50, 50));
    healthBarBackground.setPosition(10.f, 50.f);

    healthBarForeground.setSize({100.f, 15.f});
    healthBarForeground.setFillColor(sf::Color(220, 0, 0));
    healthBarForeground.setPosition(10.f, 50.f);

    float langBtnSize = 60.f;

    // EN Button
    langEnButton.setSize({langBtnSize, langBtnSize});
    langEnButton.setFillColor(sf::Color(70, 70, 70));
    langEnButton.setOutlineThickness(2);
    langEnButton.setOutlineColor(sf::Color::White);
    langEnButton.setPosition(window.getSize().x - 150.f, 20.f); // Правий верхній кут

    langEnText.setFont(font);
    langEnText.setString("EN");
    langEnText.setCharacterSize(20);
    langEnText.setFillColor(sf::Color::White);
    centerTextOrigin(langEnText);
    langEnText.setPosition(
            langEnButton.getPosition().x + langBtnSize/2,
            langEnButton.getPosition().y + langBtnSize/2
    );

    // UKR Button
    langUkrButton = langEnButton;
    langUkrButton.setPosition(window.getSize().x - 70.f, 20.f);

    langUkrText = langEnText;
    langUkrText.setString("UA");
    centerTextOrigin(langUkrText);
    langUkrText.setPosition(
            langUkrButton.getPosition().x + langBtnSize/2,
            langUkrButton.getPosition().y + langBtnSize/2
    );

    // Завантажуємо початкову мову (наприклад, EN)
    L10N.loadLanguage("en");
    updateUITexts(); // Оновлюємо тексти відразу

}


// --- ЛОГІКА ГРИ ---
void Game::resetGame() {
    LOG_INFO("Resetting game state...");


    map = Map(configMapWidth, configMapHeight, 20);


    player.reset(1, 1);
    player.chooseWeapon(1);
    enemies.clear();

// Спавн Боса у дальньому куті
    if (configEnemyCount > 0) {

        int bossX = configMapWidth - 2;
        int bossY = configMapHeight - 2;
        if (map.getGrid()[bossY][bossX] == 1) { bossX--; }

        enemies.add(make_unique<Boss>("BOSS", 120, 20, bossX, bossY, 7));
        LOG_INFO("Boss spawned at (" + to_string(bossX) + "," + to_string(bossY) + ")");
    }

// Випадковий спавн зомбі на вільних клітинах
    for (int i = 0; i < configEnemyCount - 1; ++i) {
        int z_x, z_y;
        bool validSpot = false;
        int attempts = 0;

        // Шукаємо вільне місце (до 50 спроб)
        while (!validSpot && attempts < 50) {

            z_x = 1 + rand() % (configMapWidth - 2);
            z_y = 1 + rand() % (configMapHeight - 2);


            int distToPlayer = abs(z_x - player.getX()) + abs(z_y - player.getY());
            // Перевірка: не стіна і не занадто близько до гравця
            if (map.getGrid()[z_y][z_x] != 1 && distToPlayer > 3) {
                validSpot = true;
            }
            attempts++;
        }

        if (!validSpot) {
            z_x = configMapWidth - 2;
            z_y = configMapHeight - 2;
        }

        enemies.add(make_unique<Zombie>("Zombie " + std::to_string(i + 1), 50, 10, z_x, z_y));
    }


    LOG_INFO("Total enemies active: " + to_string(enemies.size()));

    isPlayerTurn = true;
    logMessages.clear();
    addLogMessage("Game started! Press 'Q' to swap weapon.");
}
void Game::runGameLoop() {
    LOG_INFO("Entering main game loop.");
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
    LOG_INFO("Exiting main game loop.");
}


// --- ОБРОБКА ПОДІЙ ---
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            LOG_INFO("Window close event received.");
            window.close();
        }

        switch (currentState) {
            case GameState::MainMenu:       processMainMenuEvents(event); break;
            case GameState::ConfigSelection: processConfigSelectionEvents(event); break;
            case GameState::Playing:        processPlayingEvents(event);  break;
            case GameState::Paused:         processPausedEvents(event);   break;
            case GameState::GameOver:       processGameOverEvents(event); break;
        }
    }
}

void Game::update() {
    if (currentState == GameState::Playing) {
        updatePlaying();
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    switch (currentState) {
        case GameState::MainMenu:       renderMainMenu(); break;
        case GameState::ConfigSelection: renderConfigSelection(); break;
        case GameState::Playing:        renderPlaying();  break;
        case GameState::Paused:         renderPaused();   break;
        case GameState::GameOver:       renderGameOver(); break;
    }

    window.display();
}

void Game::processMainMenuEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
                {event.mouseButton.x, event.mouseButton.y},
                window.getDefaultView()
        );
        if (playButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("User selected New Game.");
            currentState = GameState::ConfigSelection;
        }
        if (exitButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("User selected Exit.");
            window.close();
        }
        if (langEnButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Language switched to English");
            L10N.loadLanguage("en");
            updateUITexts(); // Миттєве оновлення інтерфейсу
        }

        if (langUkrButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Language switched to Ukrainian");
            L10N.loadLanguage("ukr");
            updateUITexts(); // Миттєве оновлення інтерфейсу
        }
    }
}

void Game::processConfigSelectionEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
                {event.mouseButton.x, event.mouseButton.y},
                window.getDefaultView()
        );

        if (mapWidthDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapWidth > 15) configMapWidth--;
        }
        if (mapWidthIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapWidth < 30) configMapWidth++;
        }
        if (mapHeightDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapHeight > 15) configMapHeight--;
        }
        if (mapHeightIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapHeight < 30) configMapHeight++;
        }
        if (enemyCountDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configEnemyCount > 1) configEnemyCount--;
        }
        if (enemyCountIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configEnemyCount < 10) configEnemyCount++;
        }

        if (configStartButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Configuration confirmed. Starting game.");
            resetGame();
            currentState = GameState::Playing;
        }
        if (configBackButton.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::MainMenu;
        }
    }
}

void Game::processPlayingEvents(sf::Event& event) {
    if (isPlayerTurn && event.type == sf::Event::KeyPressed) {
        bool actionTaken = false;
        if (event.key.code == sf::Keyboard::W) { player.move(0, -1, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::S) { player.move(0, 1, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::A) { player.move(-1, 0, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::D) { player.move(1, 0, map.getGrid()); actionTaken = true; }
        if (event.key.code == sf::Keyboard::F) { handlePlayerAttack(); actionTaken = true; }


        if (event.key.code == sf::Keyboard::Q) {
            player.swapWeapon();
            addLogMessage("Swapped to " + player.getWeaponName());
        }

        if (event.key.code == sf::Keyboard::Escape) {
            LOG_INFO("Game paused by user.");
            currentState = GameState::Paused;
        }

        if (actionTaken && player.isAlive()) {
            isPlayerTurn = false;
        }
    }
}

void Game::processPausedEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        window.setView(window.getDefaultView());
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        if (resumeButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Game resumed.");
            currentState = GameState::Playing;
        }
        if (pauseRestartButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Game restarted from pause menu.");
            resetGame();
            currentState = GameState::Playing;
        }
        if (pauseToMenuButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Returned to Main Menu from pause.");
            currentState = GameState::MainMenu;
        }
        if (pauseExitDesktopButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Exit to desktop from pause.");
            window.close();
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        LOG_INFO("Game resumed (Escape key).");
        currentState = GameState::Playing;
    }
}

void Game::processGameOverEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (restartButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("Restarting after Game Over.");
            currentState = GameState::ConfigSelection;
        }
        if (gameOverExitButton.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::MainMenu;
        }
    }
}

void Game::centerTextOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}


// --- ОНОВЛЕННЯ СТАНУ ГРИ ---
void Game::updatePlaying() {

    //Перевірка умови перемоги
    if (currentState == GameState::Playing && enemies.size() == 0) {
        LOG_INFO("VICTORY! All enemies defeated.");
        currentState = GameState::GameOver;
        gameOverTitleText.setString("Victory!");
        centerTextOrigin(gameOverTitleText);
        addLogMessage("All enemies defeated!");
        return;
    }

    //ПІДБІР ПРЕДМЕТІВ
    int tileType = map.getGrid()[player.getY()][player.getX()];

    if (tileType == 2) { //Зілля
        LOG_INFO("Picked up Health Potion");
        player.heal(25);
        map.clearTile(player.getX(), player.getY());
        addLogMessage("Health Potion (+25 HP)");
        pickupSound.play();
    }
    else if (tileType == 3) { // Патрони
        LOG_INFO("Picked up Ammo Pack");
        player.addAmmo(5);
        map.clearTile(player.getX(), player.getY());
        addLogMessage("Ammo Pack (+5 Ammo)");
        pickupSound.play();
    }


    //Логіка ходу ворогів
    if (!isPlayerTurn && player.isAlive()) {
        const auto& allEnemiesRaw = enemies.getAllRaw();

        for (auto* e : allEnemiesRaw) {
            if (auto* z = dynamic_cast<Zombie*>(e)) {
                int dx = abs(z->getX() - player.getX());
                int dy = abs(z->getY() - player.getY());


                if (dx + dy == 1) {
                    int damage = dynamic_cast<Boss*>(z) ? 20 : 10;
                    z->attack(player);
                    hitSound.setPitch(0.8);
                    hitSound.play();
                    addLogMessage(z->getName() + " hits player for " + std::to_string(damage) + "!");

                    if (!player.isAlive()) {
                        LOG_INFO("DEFEAT. Player killed by " + z->getName());
                        addLogMessage("Player has fallen!");
                        currentState = GameState::GameOver;
                        gameOverTitleText.setString("Defeat...");
                        centerTextOrigin(gameOverTitleText);
                        break;
                    }
                }
                else {
                    z->moveTowards(player.getX(), player.getY(), map.getGrid(), allEnemiesRaw);
                }
            }
            if (currentState == GameState::GameOver) break;
        }
        isPlayerTurn = true;
    }

    // 4. Оновлення HUD
    healthText.setString("Health: " + std::to_string(player.getHealth()));
    scoreText.setString("Score: " + std::to_string(player.getScore()));
    ammoText.setString("Ammo: " + std::to_string(player.getAmmo()) + " | Weapon: " + player.getWeaponName());

    float hpPercent = static_cast<float>(player.getHealth()) / playerMaxHealth;
    if (hpPercent < 0) hpPercent = 0;
    healthBarForeground.setSize({100.f * hpPercent, 15.f});

    // 5. Оновлення камери
    const int TILE_SIZE = 32;
    float viewX = static_cast<float>(player.getX() * TILE_SIZE);
    float viewY = static_cast<float>(player.getY() * TILE_SIZE);


    gameView.setCenter(viewX, viewY);
}


// --- РЕНДЕРИНГ ---
void Game::renderMainMenu() {
    window.setView(window.getDefaultView());
    window.draw(menuTitleText);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);

    window.draw(langEnButton);
    window.draw(langEnText);
    window.draw(langUkrButton);
    window.draw(langUkrText);
}

void Game::renderConfigSelection() {
    window.setView(window.getDefaultView());

    window.draw(configTitleText);

    mapWidthText.setString(L10N.getFormattedString("config_map_width", configMapWidth));
    centerTextOrigin(mapWidthText);
    window.draw(mapWidthText);

    mapHeightText.setString(L10N.getFormattedString("config_map_height", configMapHeight));
    centerTextOrigin(mapHeightText);
    window.draw(mapHeightText);

    enemyCountText.setString(L10N.getFormattedString("config_enemies", configEnemyCount));
    centerTextOrigin(enemyCountText);
    window.draw(enemyCountText);

    window.draw(mapWidthDecreaseButton);
    window.draw(mapWidthDecreaseText);
    window.draw(mapWidthIncreaseButton);
    window.draw(mapWidthIncreaseText);
    window.draw(mapHeightDecreaseButton);
    window.draw(mapHeightDecreaseText);
    window.draw(mapHeightIncreaseButton);
    window.draw(mapHeightIncreaseText);
    window.draw(enemyCountDecreaseButton);
    window.draw(enemyCountDecreaseText);
    window.draw(enemyCountIncreaseButton);
    window.draw(enemyCountIncreaseText);
    window.draw(configStartButton);
    window.draw(configStartButtonText);
    window.draw(configBackButton);
    window.draw(configBackButtonText);
}

void Game::renderPlaying() {
    const int TILE_SIZE = 32;

    window.setView(gameView);

    //Малюємо карту
    for (int y = 0; y < configMapHeight; ++y) {
        for (int x = 0; x < configMapWidth; ++x) {
            int tileType = map.getGrid()[y][x];

            sf::Sprite tileSprite;
            tileSprite.setTexture((tileType == 1) ? wallTexture : floorTexture);

            sf::FloatRect bounds = tileSprite.getLocalBounds();
            tileSprite.setScale(
                static_cast<float>(TILE_SIZE) / bounds.width,
                static_cast<float>(TILE_SIZE) / bounds.height
            );

            tileSprite.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
            window.draw(tileSprite);

            // Зілля (2)
            if (tileType == 2) {
                sf::CircleShape potion(10.f);
                potion.setFillColor(sf::Color::Green);
                potion.setPosition(static_cast<float>(x * TILE_SIZE) + 6.f, static_cast<float>(y * TILE_SIZE) + 6.f);
                window.draw(potion);
            }
            // Патрони (3)
            else if (tileType == 3) {
                sf::RectangleShape ammoBox({14.f, 14.f});
                ammoBox.setFillColor(sf::Color::Yellow);
                ammoBox.setOutlineColor(sf::Color::Black);
                ammoBox.setOutlineThickness(1.f);
                ammoBox.setPosition(static_cast<float>(x * TILE_SIZE) + 9.f, static_cast<float>(y * TILE_SIZE) + 9.f);
                window.draw(ammoBox);
            }
        }
    }

    for (auto* e : enemies.getAllRaw()) {
        if (auto* z = dynamic_cast<Zombie*>(e)) {
            sf::Sprite enemySprite;
            enemySprite.setTexture(dynamic_cast<Boss*>(z) ? bossTexture : zombieTexture);

            sf::FloatRect bounds = enemySprite.getLocalBounds();
            enemySprite.setScale(
                static_cast<float>(TILE_SIZE) / bounds.width,
                static_cast<float>(TILE_SIZE) / bounds.height
            );

            enemySprite.setPosition(static_cast<float>(z->getX() * TILE_SIZE), static_cast<float>(z->getY() * TILE_SIZE));
            window.draw(enemySprite);
        }
    }

    sf::Sprite playerSprite(playerTexture);

    sf::FloatRect pBounds = playerSprite.getLocalBounds();
    playerSprite.setScale(
        static_cast<float>(TILE_SIZE) / pBounds.width,
        static_cast<float>(TILE_SIZE) / pBounds.height
    );

    playerSprite.setPosition(static_cast<float>(player.getX() * TILE_SIZE), static_cast<float>(player.getY() * TILE_SIZE));
    window.draw(playerSprite);

    //Рендер HUD
    window.setView(window.getDefaultView());

    window.draw(healthText);
    window.draw(scoreText);
    window.draw(ammoText);
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);

    float logY = window.getSize().y - 20.f;
    for (auto it = logMessages.rbegin(); it != logMessages.rend(); ++it) {
        it->setPosition(10.f, logY);
        window.draw(*it);
        logY -= 20.f;
    }
}

void Game::renderPaused() {
    renderPlaying();
    sf::RectangleShape overlay({(float)window.getSize().x, (float)window.getSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    window.setView(window.getDefaultView());
    window.draw(overlay);

    window.draw(pauseTitleText);
    window.draw(resumeButton);
    window.draw(resumeButtonText);
    window.draw(pauseRestartButton);
    window.draw(pauseRestartButtonText);
    window.draw(pauseToMenuButton);
    window.draw(pauseToMenuButtonText);
    window.draw(pauseExitDesktopButton);
    window.draw(pauseExitDesktopButtonText);
}

void Game::renderGameOver() {
    window.setView(window.getDefaultView());

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

void Game::handlePlayerAttack() {
    bool attacked = false;
    int weaponRange = player.getWeaponRange();

    for (size_t i = 0; i < enemies.size(); ++i) {
        if (auto* z = dynamic_cast<Zombie*>(enemies.get(i))) {

            int dx = abs(z->getX() - player.getX());
            int dy = abs(z->getY() - player.getY());
            int dist = dx + dy;

            if (dist <= weaponRange) {

                if (!player.canAttack()) {
                    addLogMessage("Click! No Ammo!");
                    continue;
                }
                if (player.getWeaponName() == "Gun") {
                    shootSound.play();
                } else {
                    hitSound.setPitch(1.5);
                    hitSound.play();
                }

                LOG_DEBUG("Player engaged enemy: " + z->getName());

                int damage = 20;
                player.attack(*z);
                addLogMessage("Player hits " + z->getName() + "!");

                if (!player.isAlive()) {
                    LOG_INFO("Player died during attack phase.");
                    addLogMessage("Player has fallen!");
                    currentState = GameState::GameOver;
                    gameOverTitleText.setString("Defeat...");
                    centerTextOrigin(gameOverTitleText);
                    break;
                }

                attacked = true;
                if (!z->isAlive()) {
                    LOG_INFO("Enemy neutralized: " + z->getName());
                    addLogMessage(z->getName() + " defeated!");
                    player.addScore(50);
                    enemies.remove(i);
                    zombieSound.play();
                }
                break;
            }
        }
    }

    if (!attacked) {
        if (player.getAmmo() <= 0 && player.getWeaponRange() > 1) {
        } else {
             addLogMessage("No enemy in range!");
        }
    }
}

void Game::addLogMessage(const std::string& message) {
    if (logMessages.size() >= MAX_LOG_MESSAGES) {
        logMessages.pop_front();
    }
    sf::Text newLog;
    newLog.setFont(font);
    newLog.setString(message);
    newLog.setCharacterSize(14);
    newLog.setFillColor(sf::Color::Green);
    logMessages.push_back(newLog);
}

void Game::updateUITexts() {
    // Головне меню
    menuTitleText.setString(L10N.getString("game_title"));
    centerTextOrigin(menuTitleText);

    playButtonText.setString(L10N.getString("menu_new_game"));
    centerTextOrigin(playButtonText);

    exitButtonText.setString(L10N.getString("menu_exit"));
    centerTextOrigin(exitButtonText);

    // Конфігурація
    configTitleText.setString(L10N.getString("config_title"));
    centerTextOrigin(configTitleText);
    configStartButtonText.setString(L10N.getString("config_start"));
    centerTextOrigin(configStartButtonText);
    configBackButtonText.setString(L10N.getString("config_back"));
    centerTextOrigin(configBackButtonText);

    // Пауза
    pauseTitleText.setString(L10N.getString("paused"));
    centerTextOrigin(pauseTitleText);
    resumeButtonText.setString(L10N.getString("resume"));
    centerTextOrigin(resumeButtonText);
    pauseRestartButtonText.setString(L10N.getString("restart"));
    centerTextOrigin(pauseRestartButtonText);
    pauseToMenuButtonText.setString(L10N.getString("to_menu"));
    centerTextOrigin(pauseToMenuButtonText);
    pauseExitDesktopButtonText.setString(L10N.getString("to_desktop"));
    centerTextOrigin(pauseExitDesktopButtonText);

    // Game Over
    if (currentState == GameState::GameOver) {
        restartButtonText.setString(L10N.getString("restart"));
        centerTextOrigin(restartButtonText);
        gameOverExitButtonText.setString(L10N.getString("to_menu"));
        centerTextOrigin(gameOverExitButtonText);
    }
}
