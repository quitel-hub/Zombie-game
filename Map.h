#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include "Entity.h"
#include "Zombie.h"
#include "Player.h"
#include "Logger.h"

using namespace std;

#ifndef UNTITLED23_MAP_H
#define UNTITLED23_MAP_H
#endif

const int TILE_FLOOR = 0;
const int TILE_WALL = 1;
const int TILE_POTION = 2;
const int TILE_AMMO = 3;

class Map {
    int width, height;
    vector<vector<int>> grid;
public:
    Map(int w, int h, int wallPercent) : width(w), height(h) {
        grid = vector<vector<int>>(h, vector<int>(w, TILE_FLOOR));

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (y == 0 || y == h - 1 || x == 0 || x == w - 1) {
                    grid[y][x] = TILE_WALL;
                } else {
                    int r = rand() % 100;
                    if (r < wallPercent) {
                        grid[y][x] = TILE_WALL;
                    }
                    else if (r < wallPercent + 3) {
                        grid[y][x] = TILE_POTION;
                    }
                    else if (r < wallPercent + 3 + 3) {
                        grid[y][x] = TILE_AMMO;
                    }
                }
            }
        }
        LOG_INFO("Map generated with Potions and Ammo.");
    }

    void clearTile(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = TILE_FLOOR;
        }
    }

    const vector<vector<int>>& getGrid() const { return grid; }

    void render(const Player& p, const vector<Entity*>& enemies) {

    }
};