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
/**
 * @brief Клас, що представляє Гравця. Успадковується від Entity.
 *
 * Містить логіку для руху, вибору зброї, атаки та підрахунку очок.
 * Координати (x, y) використовуються для логіки на карті.
 */
class Player : public Entity {
    int score;
    int x, y;
    unique_ptr<Weapon> weapon;
    bool weaponChosen;

public:
    /**
     * @brief Конструктор гравця.
     * @param n Ім'я гравця.
     * @param h Початкове здоров'я.
     * @param d Базова шкода (без зброї).
     * @param sx Початкова координата X.
     * @param sy Початкова координата Y.
     */
    Player(const std::string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), weapon(nullptr), score(0), x(sx), y(sy), weaponChosen(false) {}

    /**
         * @brief Деструктор.
         */
    ~Player() {}
    /**
     * @brief Скидає стан гравця до початкових значень (для нової гри).
     * @param startX Початкова координата X.
     * @param startY Початкова координата Y.
     */
    void reset(int startX, int startY) {
        health = 100; // Початкове здоров'я
        score = 0;    // Початковий рахунок
        x = startX;   // Початкова позиція X
        y = startY;   // Початкова позиція Y
        weapon.reset(); // Видалити поточну зброю (unique_ptr має метод reset)
        weaponChosen = false;
    }
    /**
     * @brief Встановлює зброю для гравця на основі вибору.
     * @param choice 1 для Меча (Sword), 2 для Пістолета (Gun).
     */
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
    /**
     * @brief Перевизначений метод атаки гравця.
     *
     * Якщо гравець має зброю, загальна шкода = базова шкода + шкода від зброї.
     * @param target Ціль для атаки (інша Entity).
     */
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

    /**
     * @brief Повертає символ гравця.
     * @return Символ 'P'.
     */
    char getSymbol() const override { return 'P'; }
    /**
     * @brief Додає очки до загального рахунку гравця.
     * @param points Кількість очок для додавання.
     */
    void addScore(int points) { score += points; }
    /**
     * @brief Отримує поточний рахунок гравця.
     * @return int рахунок.
     */
    int getScore() const { return score; }
    /**
     * @brief Отримує поточну координату X гравця.
     * @return int X.
     */
    int getX() const { return x; }
    /**
     * @brief Отримує поточну координату Y гравця.
     * @return int Y.
     */
    int getY() const { return y; }
    /**
     * @brief Логіка руху гравця по карті.
     *
     * Змінює координати (x, y), якщо нова позиція (nx, ny)
     * є вільною (значення 0 на карті).
     * @param dx Зміщення по X (-1, 0, 1).
     * @param dy Зміщення по Y (-1, 0, 1).
     * @param map Посилання на 2D-вектор (сітку) карти для перевірки зіткнень.
     */
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