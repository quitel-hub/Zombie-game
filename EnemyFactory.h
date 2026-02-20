#pragma once
#include "Zombie.h"
#include "Boss.h"

class EnemyFactory {
public:
    Zombie* createBasicZombie(int x, int y) {
        return new Zombie("Zombie", 50, 10, x, y);
    }

    Boss* createBossEntity(int x, int y) {
        return new Boss("Boss", 150, 20, 5, x, y);
    }
};