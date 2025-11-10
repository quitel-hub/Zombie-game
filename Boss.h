#pragma once
#include<iostream>
#include <string>
#include"Zombie.h"
#include "LocalizationManager.h"
using namespace std;

#ifndef UNTITLED23_BOSS_H
#define UNTITLED23_BOSS_H

#endif //UNTITLED23_BOSS_H

class Boss : public Zombie {
    int rage;
public:

    Boss(const std::string& n, int h, int d, int r, int sx, int sy)
            : Zombie(n, h, d, sx, sy), rage(r) {}

    void attack(Entity& target) override {
        int totalDamage = getDamage() + rage;
        cout << L10N.getString("boss_attack_header") << endl;
        cout << L10N.getFormattedString("boss_attacks_target", getName(), getHealth(), target.getName(), target.getHealth()) << endl;
        target.takeDamage(totalDamage);
        cout << L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()) << "\n";
    }
    char getSymbol() const override { return 'B'; }
};