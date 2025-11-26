#include "Weapon.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_SWORD_H
#define UNTITLED23_SWORD_H

#endif //UNTITLED23_SWORD_H

/**
 * @brief Холодна зброя (Меч).
 * Б'є лише на сусідню клітинку, патронів не потребує.
 */
class Sword : public Weapon {
public:

    Sword() : Weapon("Sword", 40) {}
};