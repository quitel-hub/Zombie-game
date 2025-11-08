#include<iostream>
#pragma once
using namespace std;

#ifndef UNTITLED23_WEAPON_H
#define UNTITLED23_WEAPON_H

#endif //UNTITLED23_WEAPON_H
/**
 * @brief Базовий клас для всієї зброї.
 *
 * Визначає основні властивості зброї: ім'я та шкоду.
 */
class Weapon {
protected:
    string name;
    int damage;
public:
    /**
     * @brief Конструктор зброї.
     * @param n Назва зброї.
     * @param d Шкода від зброї.
     */
    Weapon(string n, int d) : name(n), damage(d) {}
    /**
     * @brief Віртуальний деструктор.
     */
    virtual ~Weapon() {}
    /**
     * @brief Отримує шкоду зброї.
     * @return int шкода.
     */
    virtual int getDamage() const { return damage; }
    /**
     * @brief Отримує шкоду зброї.
     * @return int шкода.
     */
    virtual string getName() const { return name; }
};
