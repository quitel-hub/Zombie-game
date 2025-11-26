#pragma once
#include <iostream>
#include <string>
#include "Zombie.h"
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_BOSS_H
#define UNTITLED23_BOSS_H
#endif
/**
 * @brief Клас Боса - посиленого ворога.
 * @details Бос успадковує поведінку Зомбі, але має додатковий параметр люті (rage),
 * який збільшує його шкоду при кожній атаці.
 */
class Boss : public Zombie {
    int rage;
public:
    /**
     * @brief Конструктор Боса.
     * @param n Ім'я.
     * @param h Здоров'я.
     * @param d Базова шкода.
     * @param r Показник люті (додається до шкоди).
     * @param sx Початкова координата X.
     * @param sy Початкова координата Y.
     */
    Boss(const std::string& n, int h, int d, int r, int sx, int sy)
            : Zombie(n, h, d, sx, sy), rage(r) {}
    /**
     * @brief Атака Боса.
     * @details Завдає шкоди цілі, що дорівнює (базова_шкода + лють).
     * @param target Сутність, яку атакує бос.
     */
    void attack(Entity& target) override {
        int totalDamage = getDamage() + rage;

        LOG_INFO(L10N.getString("boss_attack_header"));
        LOG_INFO(L10N.getFormattedString("boss_attacks_target", getName(), getHealth(), target.getName(), target.getHealth()));

        target.takeDamage(totalDamage);

        LOG_INFO(L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()));
    }
    /**
     * @brief Повертає символ для відображення на карті ('B').
     */
    char getSymbol() const override { return 'B'; }
};