#include <benchmark/benchmark.h>
#include <vector>
#include <memory>
#include "../Zombie.h"
#include "../Map.h"
#include "../Player.h"


static void BM_ZombieMovement(benchmark::State& state) {
    // 1. Підготовка (Setup)
    // Створюємо пусту карту 100x100
    std::vector<std::vector<int>> dummyGrid(100, std::vector<int>(100, 0));
    std::vector<Entity*> emptyEnemies;

    // Зомбі в позиції (10, 10)
    Zombie zombie("Runner", 100, 10, 10, 10);

    // Ціль (Гравець) далеко, на позиції (90, 90)
    int targetX = 90;
    int targetY = 90;

    // 2. Цикл заміру (Loop)
    for (auto _ : state) {
        zombie.moveTowards(targetX, targetY, dummyGrid, emptyEnemies);

        benchmark::DoNotOptimize(zombie);
    }
}

BENCHMARK(BM_ZombieMovement);


// --- Бенчмарк 2: Генерації Карти ---
static void BM_MapGeneration(benchmark::State& state) {
    for (auto _ : state) {
        Map map(50, 50, 20);

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(BM_MapGeneration);


// --- Бенчмарк 3: Перевірка колізій (Стіни) ---
static void BM_MapAccess(benchmark::State& state) {
    Map map(100, 100, 0);
    auto& grid = map.getGrid();
    int x = 50;
    int y = 50;

    for (auto _ : state) {
        int tileType = grid[y][x];
        benchmark::DoNotOptimize(tileType);
    }
}
BENCHMARK(BM_MapAccess);

BENCHMARK_MAIN();