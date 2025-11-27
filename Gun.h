#include "Weapon.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_GUN_H
#define UNTITLED23_GUN_H
#endif
/**
 * @brief Вогнепальна зброя (Пістолет).
 * Має дальність стрільби та вимагає патронів.
 */
class Gun : public Weapon {
public:

    Gun() : Weapon("Gun", 25, 5) {}

    bool isRanged() const override { return true; }
};