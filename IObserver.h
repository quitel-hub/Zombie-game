#pragma once

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onEnemyKilled(int scoreValue) = 0;
};