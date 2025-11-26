#pragma once

class Game;

#include "Game.h"
#include "Player.h"
#include "Map.h"


/**
 * @brief Абстрактний базовий клас для патерну Command.
 * @details Використовується для інкапсуляції дій гравця (якщо планується система Replay або Undo).
 */
class Command {
public:
    virtual ~Command() = default;

    /**
     * @brief Виконує команду над грою.
     * @param game Посилання на об'єкт гри.
     */
    virtual void execute(Game& game) = 0;
};



class MoveCommand : public Command {
private:
    int dx, dy;
public:

    MoveCommand(int x, int y) : dx(x), dy(y) {}


    void execute(Game& game) override {
        game.getPlayer().move(dx, dy, game.getMap().getGrid());
    }
};


class AttackCommand : public Command {
public:

    void execute(Game& game) override {
        game.handlePlayerAttack();
    }
};


class QuitCommand : public Command {
public:

    void execute(Game& game) override {
        game.setGameOver();
    }
};