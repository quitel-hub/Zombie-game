#pragma once
#include<iostream>
#include <string>
#include"Zombie.h"
#include "LocalizationManager.h"
using namespace std;

#ifndef UNTITLED23_BOSS_H
#define UNTITLED23_BOSS_H

#endif //UNTITLED23_BOSS_H
/**
 * @brief Клас Бос. Успадковується від Зомбі.
 *
 * Сильніший ворог, який має додаткову шкоду від "люті" (rage).
 */
class Boss : public Zombie {
    int rage;
public:
    /**
     * @brief Конструктор Боса.
     * @param n Ім'я.
     * @param h Здоров'я.
     * @param d Базова шкода.
     * @param r Шкода від люті.
     * @param sx Початкова X.
     * @param sy Початкова Y.
     */
    Boss(const std::string& n, int h, int d, int r, int sx, int sy)
            : Zombie(n, h, d, sx, sy), rage(r) {}
    /**
         * @brief Перевизначений метод атаки Боса.
         * Шкода = базова шкода (від Zombie) + шкода від люті (rage).
         * @param target Ціль для атаки.
         */
    void attack(Entity& target) override {
        int totalDamage = getDamage() + rage;
        cout << L10N.getString("boss_attack_header") << endl;
        cout << L10N.getFormattedString("boss_attacks_target", getName(), getHealth(), target.getName(), target.getHealth()) << endl;
        target.takeDamage(totalDamage);
        cout << L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()) << "\n";
    }
    /**
         * @brief Повертає символ Боса.
         * @return Символ 'B'.
         */
    char getSymbol() const override { return 'B'; }
};