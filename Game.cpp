#include "Game.h"
#include "Zombie.h"
#include "Boss.h"
#include "Logger.h"
#include <string>
#include <algorithm>

// Конструктор
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
    // Ініціалізуємо ігрову камеру
    gameView.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
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

    if (!font.loadFromFile("assets/3Dumb.ttf") && !font.loadFromFile("3Dumb.ttf")) {
        LOG_ERR("CRITICAL: Could not load font '3Dumb.ttf'");
        errorOccurred = true;
    }
    if (!playerTexture.loadFromFile("assets/player.png") && !playerTexture.loadFromFile("player.png")) {
        LOG_ERR("Error loading player.png");
        errorOccurred = true;
    }
    if (!zombieTexture.loadFromFile("assets/zombie.png") && !zombieTexture.loadFromFile("zombie.png")) {
        LOG_ERR("Error loading zombie.png");
        errorOccurred = true;
    }
    if (!bossTexture.loadFromFile("assets/boss.png") && !bossTexture.loadFromFile("boss.png"))   {
        LOG_ERR("Error loading boss.png");
        errorOccurred = true;
    }
    if (!wallTexture.loadFromFile("assets/wall.png") && !wallTexture.loadFromFile("wall.png"))     {
        LOG_ERR("Error loading wall.png");
        errorOccurred = true;
    }
    if (!floorTexture.loadFromFile("assets/floor.png") && !floorTexture.loadFromFile("floor.png"))   {
        LOG_ERR("Error loading floor.png");
        errorOccurred = true;
    }

    if (!errorOccurred) {
        LOG_INFO("All assets loaded successfully.");
    } else {
        LOG_WARN("Some assets failed to load. Game may look incorrect.");
    }
}

void Game::setupUI() {
    // ... (Код налаштування UI залишається без змін, він не потребує логування) ...
    float centerX = window.getSize().x / 2.0f;
    float buttonWidth = 200.f;
    float buttonHeight = 50.f;
    sf::Color buttonColor(100, 100, 100);
    sf::Color textColor = sf::Color::Green;
    unsigned int charSize = 24;
    unsigned int titleCharSize = 50;

    // --- Головне меню ---
    menuTitleText.setFont(font);
    menuTitleText.setString("Zombie Survival");
    menuTitleText.setCharacterSize(titleCharSize);
    menuTitleText.setFillColor(sf::Color::Green);
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
    configTitleText.setPosition(centerX, window.getSize().y / 20.0f);

    float buttonSize = 40.f;
    float labelY_MapWidth = window.getSize().y / 3.0f;
    float labelY_MapHeight = labelY_MapWidth + 80.f;
    float labelY_Enemies = labelY_MapHeight + 100.0f;
    sf::Color smallButtonColor(70, 70, 70);

    // Map Width
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

    // Map Height
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

    // Enemy Count
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

    // Config Start/Back Buttons
    configStartButton = playButton;
    configStartButton.setPosition(centerX, window.getSize().y - 100.f);
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

    // --- Меню паузи ---
    pauseTitleText = menuTitleText;
    pauseTitleText.setString("Paused");
    centerTextOrigin(pauseTitleText);
    pauseTitleText.setPosition(centerX, window.getSize().y / 4.0f);

    resumeButton = playButton;
    resumeButton.setPosition(centerX, window.getSize().y / 2.0f - 35.f);
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

    // --- Екран кінця гри ---
    gameOverTitleText = menuTitleText;
    gameOverTitleText.setString("Game Over");
    centerTextOrigin(gameOverTitleText);
    gameOverTitleText.setPosition(centerX, window.getSize().y / 4.0f);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(charSize);
    finalScoreText.setFillColor(textColor);

    restartButton = playButton;
    restartButton.setPosition(centerX, window.getSize().y / 2.0f + 30.f);
    restartButtonText = playButtonText;
    restartButtonText.setString("Restart");
    centerTextOrigin(restartButtonText);
    restartButtonText.setPosition(restartButton.getPosition());

    gameOverExitButton = exitButton;
    gameOverExitButton.setPosition(centerX, restartButton.getPosition().y + 70.f);
    gameOverExitButtonText = pauseToMenuButtonText;
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

    healthBarBackground.setSize({100.f, 15.f});
    healthBarBackground.setFillColor(sf::Color(50, 50, 50));
    healthBarBackground.setPosition(10.f, 50.f);

    healthBarForeground.setSize({100.f, 15.f});
    healthBarForeground.setFillColor(sf::Color(220, 0, 0));
    healthBarForeground.setPosition(10.f, 50.f);
}

void Game::resetGame() {
    LOG_INFO("Resetting game state...");
    LOG_INFO("Configuration: Map=" + to_string(configMapWidth) + "x" + to_string(configMapHeight) +
             ", Enemies=" + to_string(configEnemyCount));

    map = Map(configMapWidth, configMapHeight, 20);

    // Логуємо згенеровану карту в файл (для відладки)
    map.render(player, enemies.getAllRaw());

    player.reset(1, 1);
    player.chooseWeapon(1);
    enemies.clear();

    if (configEnemyCount > 0) {
        enemies.add(make_unique<Boss>("BOSS", 120, 20, configMapWidth - 2, configMapHeight - 2, 7));
        LOG_INFO("Boss enemy spawned.");
    }
    for (int i = 0; i < configEnemyCount - 1; ++i) {
        int z_x = 3 + (i % (configMapWidth - 4));
        int z_y = 3 + (i / (configMapWidth - 4));
        if (z_y >= configMapHeight - 2) {
            z_x = 5;
            z_y = 5;
        }
        enemies.add(make_unique<Zombie>("Zombie " + std::to_string(i + 1), 50, 10, z_x, z_y));
    }
    LOG_INFO("Total enemies active: " + to_string(enemies.size()));

    isPlayerTurn = true;
    logMessages.clear();
    addLogMessage("Game started!");
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
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (playButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("User selected New Game.");
            currentState = GameState::ConfigSelection;
        }
        if (exitButton.getGlobalBounds().contains(mousePos)) {
            LOG_INFO("User selected Exit.");
            window.close();
        }
    }
}

void Game::processConfigSelectionEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        // --- Map Width ---
        if (mapWidthDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapWidth > 15) configMapWidth--;
        }
        if (mapWidthIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapWidth < 30) configMapWidth++;
        }
        // --- Map Height ---
        if (mapHeightDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapHeight > 15) configMapHeight--;
        }
        if (mapHeightIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configMapHeight < 30) configMapHeight++;
        }
        // --- Enemy Count ---
        if (enemyCountDecreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configEnemyCount > 1) configEnemyCount--;
        }
        if (enemyCountIncreaseButton.getGlobalBounds().contains(mousePos)) {
            if (configEnemyCount < 10) configEnemyCount++;
        }

        // --- Start/Back ---
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
        if (event.key.code == sf::Keyboard::Escape) {
            LOG_INFO("Game paused by user.");
            currentState = GameState::Paused;
        }

        if (actionTaken && player.isAlive()) {
            isPlayerTurn = false; // Передача ходу ворогам
        }
    }
}

void Game::processPausedEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
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

void Game::updatePlaying() {

    // Перевірка умови перемоги
    if (currentState == GameState::Playing && enemies.size() == 0) {
        LOG_INFO("VICTORY! All enemies defeated.");
        currentState = GameState::GameOver;
        gameOverTitleText.setString("Victory!");
        centerTextOrigin(gameOverTitleText);
        addLogMessage("All enemies defeated!");
        return;
    }

    if (!isPlayerTurn && player.isAlive()) {
        LOG_DEBUG("--- Enemy Turn Start ---");


        const auto& allEnemiesRaw = enemies.getAllRaw();

        for (auto* e : allEnemiesRaw) {
            if (auto* z = dynamic_cast<Zombie*>(e)) {
                int dx = abs(z->getX() - player.getX());
                int dy = abs(z->getY() - player.getY());


                if (dx + dy == 1) {
                    int damage = dynamic_cast<Boss*>(z) ? 20 : 10;
                    z->attack(player);
                    addLogMessage(z->getName() + " hits player!");

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

        LOG_DEBUG("--- Enemy Turn End ---");
        isPlayerTurn = true;
    }

    // 4. Оновлення HUD
    healthText.setString("Health: " + std::to_string(player.getHealth()));
    scoreText.setString("Score: " + std::to_string(player.getScore()));

    float hpPercent = static_cast<float>(player.getHealth()) / playerMaxHealth;
    if (hpPercent < 0) hpPercent = 0;
    healthBarForeground.setSize({100.f * hpPercent, 15.f});

    // 5. Оновлення камери
    const int TILE_SIZE = 32;
    float viewX = static_cast<float>(player.getX() * TILE_SIZE);
    float viewY = static_cast<float>(player.getY() * TILE_SIZE);

    float halfViewX = gameView.getSize().x / 2.f;
    float halfViewY = gameView.getSize().y / 2.f;
    float mapPixelWidth = static_cast<float>(configMapWidth * TILE_SIZE);
    float mapPixelHeight = static_cast<float>(configMapHeight * TILE_SIZE);

    viewX = std::clamp(viewX, halfViewX, mapPixelWidth - halfViewX);
    viewY = std::clamp(viewY, halfViewY, mapPixelHeight - halfViewY);

    gameView.setCenter(viewX, viewY);
}

void Game::renderMainMenu() {
    window.setView(window.getDefaultView());
    window.draw(menuTitleText);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

void Game::renderConfigSelection() {
    window.setView(window.getDefaultView());

    window.draw(configTitleText);

    mapWidthText.setString("Map Width: " + std::to_string(configMapWidth));
    centerTextOrigin(mapWidthText);
    window.draw(mapWidthText);

    mapHeightText.setString("Map Height: " + std::to_string(configMapHeight));
    centerTextOrigin(mapHeightText);
    window.draw(mapHeightText);

    enemyCountText.setString("Enemies: " + std::to_string(configEnemyCount));
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

    // --- 1. Рендер ігрового світу ---
    window.setView(gameView);

    for (int y = 0; y < configMapHeight; ++y) {
        for (int x = 0; x < configMapWidth; ++x) {
            sf::Sprite tileSprite;
            tileSprite.setTexture((map.getGrid()[y][x] == 1) ? wallTexture : floorTexture);
            tileSprite.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
            window.draw(tileSprite);
        }
    }

    for (auto* e : enemies.getAllRaw()) {
        if (auto* z = dynamic_cast<Zombie*>(e)) {
            sf::Sprite enemySprite;
            enemySprite.setTexture(dynamic_cast<Boss*>(z) ? bossTexture : zombieTexture);
            enemySprite.setPosition(static_cast<float>(z->getX() * TILE_SIZE), static_cast<float>(z->getY() * TILE_SIZE));
            window.draw(enemySprite);
        }
    }

    sf::Sprite playerSprite(playerTexture);
    playerSprite.setPosition(static_cast<float>(player.getX() * TILE_SIZE), static_cast<float>(player.getY() * TILE_SIZE));
    window.draw(playerSprite);

    // --- 2. Рендер HUD ---
    window.setView(window.getDefaultView());

    window.draw(healthText);
    window.draw(scoreText);
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
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (auto* z = dynamic_cast<Zombie*>(enemies.get(i))) {
            int dx = abs(z->getX() - player.getX());
            int dy = abs(z->getY() - player.getY());
            if (dx + dy == 1) { // Ворог у зоні досяжності

                LOG_DEBUG("Player engaged enemy: " + z->getName());

                int damage = 20;
                player.attack(*z); // LOG_INFO про атаку вже всередині player.attack
                addLogMessage("Player hits " + z->getName() + " for " + std::to_string(damage) + "!");

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
                }
                break;
            }
        }
    }
    if (!attacked) {
        addLogMessage("No enemy in range!");
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
    newLog.setFillColor(sf::Color::Green); // Змінив на зелений для стилю
    logMessages.push_back(newLog);
}