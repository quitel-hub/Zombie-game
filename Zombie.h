#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "LocalizationManager.h"
#include "Entity.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_ZOMBIE_H
#define UNTITLED23_ZOMBIE_H
#endif

class Zombie : public Entity {
    int x, y;

public:
    Zombie() : Entity("Zombie", 50, 10), x(0), y(0) {}

    Zombie(const string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), x(sx), y(sy) {}

    void attack(Entity& target) override {
        LOG_INFO(L10N.getString("zombie_attack_header"));
        LOG_INFO(L10N.getFormattedString("zombie_bites_target", name, health, target.getName(), target.getHealth()));

        target.takeDamage(damage);

        LOG_INFO(L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()));
    }


    void moveTowards(int targetX, int targetY, const vector<vector<int>>& mapGrid, const vector<Entity*>& allEnemies) {
        int dx = targetX - x;
        int dy = targetY - y;

        int nextX = x;
        int nextY = y;

        bool tryHorizontal = abs(dx) > abs(dy);

        if (tryHorizontal) {
            nextX += (dx > 0) ? 1 : -1;
        } else {
            nextY += (dy > 0) ? 1 : -1;
        }

        if (nextX < 0 || nextX >= (int)mapGrid[0].size() || nextY < 0 || nextY >= (int)mapGrid.size() || mapGrid[nextY][nextX] != 0) {
            LOG_DEBUG(name + " hit a wall at (" + to_string(nextX) + "," + to_string(nextY) + "). Trying alternative path.");

            nextX = x;
            nextY = y;

            if (tryHorizontal) {
                if (dy != 0) nextY += (dy > 0) ? 1 : -1;
            } else {
                if (dx != 0) nextX += (dx > 0) ? 1 : -1;
            }

            if (mapGrid[nextY][nextX] != 0) {
                LOG_DEBUG(name + " is stuck (alternative path blocked).");
                return;
            }
        }


        for (const auto* enemy : allEnemies) {
            if (const auto* z = dynamic_cast<const Zombie*>(enemy)) {
                if (z == this) continue;
                if (z->getX() == nextX && z->getY() == nextY) {
                    LOG_DEBUG(name + " blocked by another enemy at (" + to_string(nextX) + "," + to_string(nextY) + ")");
                    return;
                }
            }
        }


        if (x != nextX || y != nextY) {
            x = nextX;
            y = nextY;
        }
    }

    char getSymbol() const override { return 'Z'; }
    int getX() const { return x; }
    int getY() const { return y; }
};