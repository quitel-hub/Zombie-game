#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"Entity.h"
#include "Weapon.h"
#include "Sword.h"
#include "Gun.h"
#include "LocalizationManager.h"
using namespace std;

#ifndef UNTITLED23_PLAYER_H
#define UNTITLED23_PLAYER_H

#endif //UNTITLED23_PLAYER_H

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
        health = 100; // Початкове здоров'я
        score = 0;    // Початковий рахунок
        x = startX;   // Початкова позиція X
        y = startY;   // Початкова позиція Y
        weapon.reset(); // Видалити поточну зброю (unique_ptr має метод reset)
        weaponChosen = false;
    }

    void chooseWeapon(int choice) {
        if (weaponChosen) {
            cout << L10N.getFormattedString("weapon_already_chosen", weapon->getName())  << endl;
            return;
        }


        if (choice == 1) weapon = make_unique<Sword>();
        else if (choice == 2) weapon = make_unique<Gun>();
        else {
            cout << L10N.getString("invalid_weapon_choice") << endl;
            weapon = make_unique<Sword>();
        }

        weaponChosen = true;
        cout <<L10N.getFormattedString("player_equipped", name, weapon->getName()) << endl;
    }

    void attack(Entity& target) override {
        cout << L10N.getString("player_attack_header") << endl;
        cout << L10N.getFormattedString("player_attacks_target", name, health, target.getName(), target.getHealth()) << target.getName() << endl;

        if (!weapon) {
            cout << L10N.getFormattedString("player_no_weapon", name) << endl;
            return;
        }

        int totalDamage = damage + weapon->getDamage();
        cout << L10N.getFormattedString("player_deals_damage", totalDamage, weapon->getName()) << endl;
        target.takeDamage(totalDamage);
        cout <<  L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()) << "\n";
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
            } else {
                cout <<  L10N.getString("cant_move_wall") << endl;
            }
        }
    }
};