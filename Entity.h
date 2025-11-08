#pragma once
#include<iostream>
#include<string>
#include "LocalizationManager.h"
using namespace std;

#ifndef UNTITLED23_ENTITY_H
#define UNTITLED23_ENTITY_H

#endif //UNTITLED23_ENTITY_H

/**
 * @brief Базовий абстрактний клас для всіх ігрових "сутностей" (Гравець, Вороги).
 *
 * Визначає загальні властивості, такі як ім'я, здоров'я, шкода,
 * та інтерфейс для взаємодії (атака, отримання шкоди).
 */


class Entity {
protected:
    string name;
    int health;
    int damage;
public:

    /**
     * @brief Конструктор сутності.
     * @param n Ім'я сутності.
     * @param h Початковий рівень здоров'я.
     * @param d Базова шкода.
     */

    Entity(const string& n, int h, int d) : name(n), health(h), damage(d) {}

    /**
     * @brief Віртуальний деструктор.
     */

    virtual ~Entity() {}


    Entity();

    /**
     * @brief Абстрактний метод атаки однієї сутності на іншу.
     * @param target Сутність, яка є ціллю атаки.
     */

    virtual void attack(Entity& target) = 0;
    /**
     * @brief Абстрактний метод для отримання символу сутності.
     * @return Символ, що представляє сутність (наприклад, 'P' для гравця).
     */
    virtual char getSymbol() const = 0;

    /**
     * @brief Метод для отримання шкоди сутністю.
     * Зменшує здоров'я на вказану величину.
     * @param dmg Кількість шкоди.
     */

    void takeDamage(int dmg) {
        cout << L10N.getFormattedString("entity_takes_damage", name, dmg) << endl;
        health -= dmg;
        if (health < 0) health = 0;
    }
    /**
     * @brief Перевіряє, чи сутність ще жива.
     * @return true, якщо здоров'я > 0, інакше false.
     */
    bool isAlive() const { return health > 0; }
    /**
         * @brief Повертає ім'я сутності.
         * @return std::string ім'я.
         */
    string getName() const { return name; }
    /**
     * @brief Повертає поточне здоров'я сутності.
     * @return int поточне здоров'я.
     */
    int getHealth() const { return health; }
    /**
     * @brief Повертає базову шкоду сутності.
     * @return int базова шкода.
     */
    int getDamage() const { return damage; }
};