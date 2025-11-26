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
    int maxHealth = 100;
    int ammo = 10;

public:
    Player(const std::string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), weapon(nullptr), score(0), x(sx), y(sy), weaponChosen(false) {}

    ~Player() {}

    void reset(int startX, int startY) {
        health = 100;
        score = 0;
        ammo = 10;
        x = startX;
        y = startY;
        weapon.reset();
        weaponChosen = false;
        LOG_INFO("Player stats reset.");
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        LOG_INFO("Player healed. HP: " + to_string(health));
    }

    void addAmmo(int amount) {
        ammo += amount;
        LOG_INFO("Ammo collected: +" + to_string(amount));
    }

    int getAmmo() const { return ammo; }


    void swapWeapon() {

        if (weapon && weapon->getName() == "Sword") {
            weapon = make_unique<Gun>();
        }

        else {
            weapon = make_unique<Sword>();
        }
        LOG_INFO("Swapped weapon to: " + weapon->getName());
    }

    string getWeaponName() const {
        return weapon ? weapon->getName() : "None";
    }


    void chooseWeapon(int choice) {
        if (choice == 1) weapon = make_unique<Sword>();
        else weapon = make_unique<Gun>();
        weaponChosen = true;
        LOG_INFO(L10N.getFormattedString("player_equipped", name, weapon->getName()));
    }

    bool canAttack() const {
        if (!weapon) return false;
        if (weapon->isRanged() && ammo <= 0) return false;
        return true;
    }

    void attack(Entity& target) override {
        if (!weapon) return;

        if (weapon->isRanged()) {
            if (ammo > 0) {
                ammo--;
                LOG_INFO("Shot fired! Ammo left: " + to_string(ammo));
            } else {
                return;
            }
        }

        int totalDamage = damage + weapon->getDamage();
        LOG_INFO(L10N.getFormattedString("player_deals_damage", totalDamage, weapon->getName()));
        target.takeDamage(totalDamage);
    }

    char getSymbol() const override { return 'P'; }
    void addScore(int points) { score += points; }
    int getScore() const { return score; }
    int getX() const { return x; }
    int getY() const { return y; }

    int getWeaponRange() const {
        return weapon ? weapon->getRange() : 1;
    }

    void move(int dx, int dy, const vector<vector<int>>& map) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && nx < (int)map[0].size() && ny >= 0 && ny < (int)map.size()) {
            if (map[ny][nx] != 1) {
                x = nx;
                y = ny;
            } else {
                LOG_INFO(L10N.getString("cant_move_wall"));
            }
        }
    }
};