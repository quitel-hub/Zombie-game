#pragma once

class Game; // Попереднє оголошення

#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "LocalizationManager.h"


class Command {
public:
    virtual ~Command() = default;


    virtual void execute(Game& game) = 0;
};

// --- Конкретні команди ---

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