#pragma once
#include<iostream>
#include<string>
#include "LocalizationManager.h"
using namespace std;

#ifndef UNTITLED23_ENTITY_H
#define UNTITLED23_ENTITY_H

#endif //UNTITLED23_ENTITY_H




class Entity {
protected:
    string name;
    int health;
    int damage;
public:



    Entity(const string& n, int h, int d) : name(n), health(h), damage(d) {}
    virtual ~Entity() {}
    Entity();

    virtual void attack(Entity& target) = 0;
    virtual char getSymbol() const = 0;

    void takeDamage(int dmg) {
        cout << L10N.getFormattedString("entity_takes_damage", name, dmg) << endl;
        health -= dmg;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }

    string getName() const { return name; }

    int getHealth() const { return health; }

    int getDamage() const { return damage; }
};