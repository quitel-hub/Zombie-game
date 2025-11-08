#include "Weapon.h"
#include <iostream>
#include <vector>
#include "LocalizationManager.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_INVENTORY_H
#define UNTITLED23_INVENTORY_H

#endif //UNTITLED23_INVENTORY_H

/**
 * @brief Клас Інвентар (зараз не використовується у Game.h).
 *
 * (Примітка: цей клас не використовується у вашому `Game.h`, але ось документація до нього).
 * Призначений для зберігання вказівників на зброю (Weapon).
 * @warning Поточна реалізація зберігає "сирі" вказівники, що може
 * призвести до витоків пам'яті. Краще використовувати `std::vector<std::unique_ptr<Weapon>>`.
 */
class Inventory {
    vector<Weapon*> items; ///< Вектор "сирих" вказівників на зброю.
public:
    /**
     * @brief Додає предмет (зброю) до інвентаря.
     * @param w Вказівник на зброю.
     */
    void addItem(Weapon* w) {
        items.push_back(w);
        cout << "Added item: " << w->getName() << endl;
    }

    /**
     * @brief Отримує предмет за індексом.
     * @param index Індекс предмета у векторі.
     * @return Вказівник на зброю або nullptr, якщо індекс недійсний.
     */
    Weapon* getItem(int index) {
        if (index >= 0 && index < items.size()) return items[index];
        return nullptr;
    }
};