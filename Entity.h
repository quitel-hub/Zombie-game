#pragma once
#include <iostream>
#include <string>
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_ENTITY_H
#define UNTITLED23_ENTITY_H
#endif
/**
 * @brief Абстрактний базовий клас для всіх сутностей гри (Гравець, Зомбі).
 */
class Entity {
protected:
    string name; ///< Ім'я сутності
    int health; ///< Поточне здоров'я
    int damage; ///< Базова сила атаки
public:
    /**
     * @brief Конструктор сутності.
     * @param n Ім'я.
     * @param h Початкове здоров'я.
     * @param d Шкода.
     */
    Entity(const string& n, int h, int d) : name(n), health(h), damage(d) {}
    virtual ~Entity() {}
    Entity();
    /**
     * @brief Чисто віртуальний метод атаки.
     * @param target Посилання на сутність, яку атакують.
     */
    virtual void attack(Entity& target) = 0;
    /**
     * @brief Отримання символу для текстового відображення.
     */
    virtual char getSymbol() const = 0;
    /**
     * @brief Метод отримання шкоди.
     * @param dmg Кількість одиниць шкоди.
     */
    void takeDamage(int dmg) {
        LOG_INFO(L10N.getFormattedString("entity_takes_damage", name, dmg));

        health -= dmg;
        if (health < 0) health = 0;

        if (health == 0) {
            LOG_INFO(name + " has died.");
        }
    }

    bool isAlive() const { return health > 0; }
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getDamage() const { return damage; }
};