#pragma once
#include <iostream>
using namespace std;

#ifndef UNTITLED23_WEAPON_H
#define UNTITLED23_WEAPON_H
#endif

class Weapon {
protected:
    string name;
    int damage;
    int range;

public:
    Weapon(string n, int d, int r = 1) : name(n), damage(d), range(r) {}

    virtual ~Weapon() {}

    virtual int getDamage() const { return damage; }
    virtual string getName() const { return name; }
    virtual int getRange() const { return range; }


    virtual bool isRanged() const { return false; }
};