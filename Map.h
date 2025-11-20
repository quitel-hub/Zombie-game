#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>
#include <map>
#include <sstream>
#include "Entity.h"
#include "Zombie.h"
#include "Player.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_MAP_H
#define UNTITLED23_MAP_H
#endif

class Map {
    int width, height;
    vector<vector<int>> grid;
public:
    Map(int w, int h, int wallPercent) : width(w), height(h) {
        grid = vector<vector<int>>(h, vector<int>(w, 0));
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (y == 0 || y == h - 1 || x == 0 || x == w - 1)
                    grid[y][x] = 1;
                else
                    grid[y][x] = (rand() % 100 < wallPercent) ? 1 : 0;
            }
        }
    }

    const vector<vector<int>>& getGrid() const { return grid; }

    // Этот метод теперь логирует карту в файл, а не печатает в консоль
    void render(const Player& p, const vector<Entity*>& enemies) {
        stringstream ss;
        ss << "\n--- MAP SNAPSHOT ---\n";

        map<pair<int, int>, char> enemy_positions;
        for (auto e : enemies) {
            if (auto z = dynamic_cast<const Zombie*>(e)) {
                enemy_positions[{z->getX(), z->getY()}] = e->getSymbol();
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == p.getX() && y == p.getY()) {
                    ss << p.getSymbol();
                } else {
                    auto it = enemy_positions.find({x, y});
                    if (it != enemy_positions.end()) {
                        ss << it->second;
                    } else {
                        ss << (grid[y][x] == 1 ? '#' : '.');
                    }
                }
            }
            ss << "\n";
        }
        ss << "--------------------\n";

        // Записываем карту как DEBUG информацию
        LOG_DEBUG(ss.str());
    }
};