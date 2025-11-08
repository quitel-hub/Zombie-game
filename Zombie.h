#pragma once
#include<iostream>
#include "LocalizationManager.h"
using namespace std;
#include"Entity.h"

#ifndef UNTITLED23_ZOMBIE_H
#define UNTITLED23_ZOMBIE_H

#endif //UNTITLED23_ZOMBIE_H
/**
 * @brief Клас Зомбі. Успадковується від Entity.
 *
 * Базовий клас для ворогів, також має координати (x, y).
 */
class Zombie : public Entity {
    int x, y;

public:
    /**
     * @brief Конструктор за замовчуванням.
     * Ініціалізує зомбі зі здоров'ям 50 та шкодою 10.
     */
    Zombie() : Entity("Zombie", 50, 10), x(0), y(0) {}
    /**
     * @brief Конструктор з параметрами.
     * @param n Ім'я.
     * @param h Здоров'я.
     * @param d Шкода.
     * @param sx Початкова X.
     * @param sy Початкова Y.
     */
    Zombie(const string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), x(sx), y(sy) {}
    /**
         * @brief Перевизначений метод атаки зомбі.
         * @param target Ціль для атаки (зазвичай гравець).
         */
    void attack(Entity& target) override {
        cout << L10N.getString("zombie_attack_header") << endl;
        cout << L10N.getFormattedString("zombie_bites_target", name, health, target.getName(), target.getHealth())  << endl;
        target.takeDamage(damage);
        cout << L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()) << "\n";
    }

    /**
         * @brief Повертає символ зомбі.
         * @return Символ 'Z'.
         */
    char getSymbol() const override { return 'Z'; }
    /**
     * @brief Отримує поточну координату X зомбі.
     * @return int X.
     */
    int getX() const { return x; }
    /**
     * @brief Отримує поточну координату Y зомбі.
     * @return int Y.
     */
    int getY() const { return y; }
};