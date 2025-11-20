#pragma once
#include <iostream>
#include <string>
#include "Zombie.h"
#include "LocalizationManager.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_BOSS_H
#define UNTITLED23_BOSS_H
#endif

class Boss : public Zombie {
    int rage;
public:
    Boss(const std::string& n, int h, int d, int r, int sx, int sy)
            : Zombie(n, h, d, sx, sy), rage(r) {}

    void attack(Entity& target) override {
        int totalDamage = getDamage() + rage;

        LOG_INFO(L10N.getString("boss_attack_header"));
        LOG_INFO(L10N.getFormattedString("boss_attacks_target", getName(), getHealth(), target.getName(), target.getHealth()));

        target.takeDamage(totalDamage);

        LOG_INFO(L10N.getFormattedString("target_hp_remaining", target.getName(), target.getHealth()));
    }
    char getSymbol() const override { return 'B'; }
};