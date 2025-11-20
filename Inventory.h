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

class Inventory {
    vector<Weapon*> items;
public:
    void addItem(Weapon* w) {
        items.push_back(w);
        LOG_INFO("Inventory added item: " + w->getName());
    }

    Weapon* getItem(int index) {
        if (index >= 0 && index < items.size()) return items[index];
        return nullptr;
    }
};