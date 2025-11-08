#pragma once

class Game; // Попереднє оголошення

#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "LocalizationManager.h"

/**
 * @brief Базовий інтерфейс для паттерну "Команда".
 *
 * Визначає єдиний метод execute, який приймає об'єкт Game
 * для виконання певної дії.
 */
class Command {
public:
    virtual ~Command() = default;

    /**
     * @brief Віртуальний метод виконання команди.
     * @param game Посилання на головний об'єкт Game, над яким виконується дія.
     */
    virtual void execute(Game& game) = 0;
};

// --- Конкретні команди ---

/**
 * @brief Команда для руху гравця.
 */
class MoveCommand : public Command {
private:
    int dx, dy; ///< Зміщення по X та Y
public:
    /**
     * @brief Конструктор команди руху.
     * @param x Зміщення по X.
     * @param y Зміщення по Y.
     */
    MoveCommand(int x, int y) : dx(x), dy(y) {}

    /**
     * @brief Виконує рух гравця.
     * @param game Посилання на Game.
     */
    void execute(Game& game) override {
        game.getPlayer().move(dx, dy, game.getMap().getGrid());
    }
};

/**
 * @brief Команда для атаки гравця.
 */
class AttackCommand : public Command {
public:
    /**
     * @brief Виконує логіку атаки гравця.
     * @param game Посилання на Game.
     */
    void execute(Game& game) override {
        game.handlePlayerAttack();
    }
};

/**
 * @brief Команда для виходу з гри (завершення).
 */
class QuitCommand : public Command {
public:
    /**
     * @brief Встановлює стан гри "GameOver".
     * @param game Посилання на Game.
     */
    void execute(Game& game) override {
        game.setGameOver();
    }
};