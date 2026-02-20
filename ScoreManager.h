#pragma once
#include "IObserver.h"
#include <iostream>

class ScoreManager : public IObserver {
private:
    int currentScore;

public:
    ScoreManager() : currentScore(0) {}

    void onEnemyKilled(int scoreValue) override {
        currentScore += scoreValue;
        // Виведення англійською, як ти і просив раніше
        std::cout << "Enemy killed! Current score: " << currentScore << std::endl;
    }

    int getCurrentScore() const {
        return currentScore;
    }
};