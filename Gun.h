#include "Weapon.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_GUN_H
#define UNTITLED23_GUN_H

#endif //UNTITLED23_GUN_H

/**
 * @brief Клас Пістолет (Gun). Успадковується від Weapon.
 *
 * Конкретна реалізація зброї зі шкодою 25.
 */
class Gun : public Weapon {
public:
    /**
     * @brief Конструктор Пістолета.
     * Встановлює назву "Gun" та шкоду 25.
     */
    Gun() : Weapon("Gun", 25) {}
};