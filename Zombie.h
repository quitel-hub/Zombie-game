#pragma once
#include<iostream>
#include "LocalizationManager.h"
using namespace std;
#include"Entity.h"

#ifndef UNTITLED23_ZOMBIE_H
#define UNTITLED23_ZOMBIE_H

#endif //UNTITLED23_ZOMBIE_H

class Zombie : public Entity {
    int x, y;

public:

    Zombie() : Entity("Zombie", 50, 10), x(0), y(0) {}

    Zombie(const string& n, int h, int d, int sx, int sy)
            : Entity(n, h, d), x(sx), y(sy) {}

    void attack(Entity& target) override {
        cout << L10N.getString("zombie_attack_header") << endl;
        cout << L10N.getFormattedString("zombie_bites_target", name, health, target.getName(), target.getHealth())  << endl;
        target.takeDamage(damage);
        cout << L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()) << "\n";
    }


    char getSymbol() const override { return 'Z'; }

    int getX() const { return x; }

    int getY() const { return y; }
};