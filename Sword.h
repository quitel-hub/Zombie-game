#include "Weapon.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_SWORD_H
#define UNTITLED23_SWORD_H

#endif //UNTITLED23_SWORD_H

/**
 * @brief Клас Меч (Sword). Успадковується від Weapon.
 *
 * Конкретна реалізація зброї зі шкодою 40.
 */
class Sword : public Weapon {
public:
    /**
     * @brief Конструктор Меча.
     * Встановлює назву "Sword" та шкоду 40.
     */
    Sword() : Weapon("Sword", 40) {}
};