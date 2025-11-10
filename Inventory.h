#include "Weapon.h"
#include <iostream>
#include <vector>
#include "LocalizationManager.h"
#pragma once
using namespace std;

#ifndef UNTITLED23_INVENTORY_H
#define UNTITLED23_INVENTORY_H

#endif //UNTITLED23_INVENTORY_H


class Inventory {
    vector<Weapon*> items;
public:

    void addItem(Weapon* w) {
        items.push_back(w);
        cout << "Added item: " << w->getName() << endl;
    }


    Weapon* getItem(int index) {
        if (index >= 0 && index < items.size()) return items[index];
        return nullptr;
    }
};