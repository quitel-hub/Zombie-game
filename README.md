# Zombie Game - Branch: feature/tests-final

Ця гілка містить реалізацію **Unit-тестування** та **Microbenchmarking** (вимірювання продуктивності) для курсового проєкту гри "Zombie Game".

## 🎯 Реалізований функціонал

1.  **Unit Tests (Google Test):**
  * Покриття тестами бойової системи (розрахунок шкоди, смерть сутностей).
  * Тестування логіки переміщення гравця та колізій.
  * Тестування AI зомбі (пошук шляху).
  * Тестування інвентарю та механік лікування/патронів.
  * Перевірка роботи контейнера сутностей та локалізації.

2.  **Microbenchmarking (Google Benchmark):**
  * Інтеграція професійної бібліотеки Google Benchmark через CMake (`FetchContent`).
  * Вимірювання швидкості алгоритму пошуку шляху зомбі (`Zombie::moveTowards`).
  * Вимірювання часу генерації процедурної карти (`Map Generation`).
  * Вимірювання швидкості доступу до даних карти (`Map Access`).

## 🛠 Як зібрати проєкт

Для коректних результатів бенчмаркінгу важливо збирати проєкт у режимі **Release** (оптимізований код).

### Крок 1: Конфігурація
# Створіть папку для збірки
```bash
mkdir cmake-build-release
cd cmake-build-release
```
# Згенеруйте файли збірки у режимі Release
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```
### Крок 2: Збірка
```bash
Зберіть всі цілі [гру, тести та бенчмарки]
cmake --build . --config Release
```
Примітка: При першій збірці CMake автоматично завантажить бібліотеки Google Test та Google Benchmark.

## 🚀 Як запустити
1. Запуск Microbenchmarks
Цей тест показує продуктивність критичних частин коду в наносекундах.
```bash
./game_benchmarks
# або для Windows:
game_benchmarks.exe
```
Очікувані результати (приблизні):

### BM_ZombieMovement: ~4-5 ns (надшвидкий алгоритм руху).
### BM_MapGeneration: ~150-300 µs (залежить від розміру карти).
### BM_MapAccess: ~0.2 ns (миттєвий доступ до пам'яті).

2. Запуск Unit Tests
Цей набір тестів перевіряє коректність логіки гри.
```bash
./unit_tests
# або для Windows:
unit_tests.exe
```
## 📂 Структура тестів
### tests/GameLogicTests.cpp: Основний файл з unit-тестами. Використовує фреймворк gtest.
### tests/Benchmarks.cpp: Файл із мікробенчмарками. Використовує бібліотеку benchmark від Google. Не містить зайвих залежностей.
### CMakeLists.txt: Налаштування збірки, що автоматично підтягує залежності.
### Як додати це в репозиторій:

1.  Створіть або відкрийте файл `README.md` у CLion.
2.  Вставте туди текст вище.
3.  Виконайте команди в терміналі:

```bash
git add README.md
git commit -m "Updated README with testing and benchmarking instructions"
git push
```
