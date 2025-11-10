#include <iostream>
#include <vector>
#include <cstdlib> // Для rand()
#include <utility> // Для std::pair
#include <map>     // Для std::map
#include "Entity.h"
#include "Zombie.h"
#include "Player.h"
#pragma once

using namespace std;
#ifndef UNTITLED23_MAP_H
#define UNTITLED23_MAP_H

#endif //UNTITLED23_MAP_H


class Map {
    int width, height;
    vector<vector<int>> grid;
public:

    Map(int w, int h, int wallPercent) : width(w), height(h) {
        grid = vector<vector<int>>(h, vector<int>(w, 0));
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (y == 0 || y == h - 1 || x == 0 || x == w - 1)
                    grid[y][x] = 1; // Стіни по периметру
                else
                    grid[y][x] = (rand() % 100 < wallPercent) ? 1 : 0; // Випадкові стіни
            }
        }
    }


    const vector<vector<int>>& getGrid() const { return grid; }


    void render(const Player& p, const vector<Entity*>& enemies) {
        map<pair<int, int>, char> enemy_positions;
        for (auto e : enemies) {
            if (auto z = dynamic_cast<const Zombie*>(e)) {
                enemy_positions[{z->getX(), z->getY()}] = e->getSymbol();
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == p.getX() && y == p.getY()) {
                    cout << p.getSymbol();
                } else {
                    auto it = enemy_positions.find({x, y});
                    if (it != enemy_positions.end()) {
                        cout << it->second;
                    } else {
                        cout << (grid[y][x] == 1 ? '#' : '.');
                    }
                }
            }
            cout << endl;
        }
    }
};