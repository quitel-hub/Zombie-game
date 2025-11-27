#pragma once
#include "Weapon.h"
#include <iostream>
#include <vector>
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_INVENTORY_H
#define UNTITLED23_INVENTORY_H
#endif
/**
 * @brief Клас інвентаря для зберігання предметів (зброї).
 */
class Inventory {
    vector<Weapon*> items;  ///< Список предметів
public:
    /**
     * @brief Додає зброю до інвентаря.
     * @param w Вказівник на об'єкт зброї.
     */
    void addItem(Weapon* w) {
        items.push_back(w);
        LOG_INFO("Inventory added item: " + w->getName());
    }
    /**
     * @brief Отримує предмет за індексом.
     * @param index Індекс предмета.
     * @return Вказівник на зброю або nullptr.
     */
    Weapon* getItem(int index) {
        if (index >= 0 && index < items.size()) return items[index];
        return nullptr;
    }
};