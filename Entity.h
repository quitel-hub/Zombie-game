#pragma once
#include <iostream>
#include <string>
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_ENTITY_H
#define UNTITLED23_ENTITY_H
#endif

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
        LOG_INFO(L10N.getFormattedString("entity_takes_damage", name, dmg));

        health -= dmg;
        if (health < 0) health = 0;

        if (health == 0) {
            LOG_INFO(name + " has died.");
        }
    }

    bool isAlive() const { return health > 0; }
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getDamage() const { return damage; }
};