#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Entity.h"
#include "Weapon.h"
#include "Sword.h"
#include "Gun.h"
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_PLAYER_H
#define UNTITLED23_PLAYER_H
#endif

class Player : public Entity {
    int score;
    int x, y;
    unique_ptr<Weapon> weapon;
    bool weaponChosen;

public:
    Player(const std::string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), weapon(nullptr), score(0), x(sx), y(sy), weaponChosen(false) {}

    ~Player() {}

    void reset(int startX, int startY) {
        health = 100;
        score = 0;
        x = startX;
        y = startY;
        weapon.reset();
        weaponChosen = false;
        LOG_INFO("Player stats reset to defaults.");
    }

    void chooseWeapon(int choice) {
        if (weaponChosen) {
            LOG_WARN(L10N.getFormattedString("weapon_already_chosen", weapon->getName()));
            return;
        }

        if (choice == 1) weapon = make_unique<Sword>();
        else if (choice == 2) weapon = make_unique<Gun>();
        else {
            LOG_WARN(L10N.getString("invalid_weapon_choice"));
            weapon = make_unique<Sword>();
        }

        weaponChosen = true;
        LOG_INFO(L10N.getFormattedString("player_equipped", name, weapon->getName()));
    }

    void attack(Entity& target) override {
        LOG_INFO(L10N.getString("player_attack_header"));
        LOG_INFO(L10N.getFormattedString("player_attacks_target", name, health, target.getName(), target.getHealth()));

        if (!weapon) {
            LOG_WARN(L10N.getFormattedString("player_no_weapon", name));
            return;
        }

        int totalDamage = damage + weapon->getDamage();
        LOG_INFO(L10N.getFormattedString("player_deals_damage", totalDamage, weapon->getName()));

        target.takeDamage(totalDamage);

        LOG_INFO(L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()));
    }

    char getSymbol() const override { return 'P'; }
    void addScore(int points) { score += points; }
    int getScore() const { return score; }
    int getX() const { return x; }
    int getY() const { return y; }

    void move(int dx, int dy, const vector<vector<int>>& map) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && nx < (int)map[0].size() && ny >= 0 && ny < (int)map.size()) {
            if (map[ny][nx] == 0) {
                x = nx;
                y = ny;
                // Можно раскомментировать для детального лога движений
                // LOG_DEBUG("Player moved to (" + to_string(x) + ", " + to_string(y) + ")");
            } else {
                LOG_INFO(L10N.getString("cant_move_wall"));
            }
        }
    }
};