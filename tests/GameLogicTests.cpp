#include "gtest/gtest.h"
#include "../Player.h"
#include "../Zombie.h"
#include "../Boss.h"
#include "../Map.h"
#include "../Container.h"
#include "../Inventory.h" // Добавили хедер Инвентаря
#include <vector>
#include <fstream> // Для тестов локализации

// --- СУЩЕСТВУЮЩИЕ ТЕСТЫ (COMBAT & BASIC MOVEMENT) ---

// Тест 1: Перевірка правильного розрахунку шкоди for player
TEST(CombatLogic, PlayerDealsCorrectDamageWithSword) {
    Player player("Hero", 100, 20, 1, 1);
    player.chooseWeapon(1); // Меч, шкода 40
    Zombie zombie("Target", 80, 10, 1, 2);
    int expectedHealth = 80 - (20 + 40);

    player.attack(zombie);

    ASSERT_EQ(zombie.getHealth(), expectedHealth);
}

// Тест 2: Перевірка, що entity помирає від летальної шкоди
TEST(CombatLogic, EntityDiesFromFatalDamage) {
    Zombie zombie("Target", 50, 10, 1, 2);
    zombie.takeDamage(100);

    ASSERT_EQ(zombie.getHealth(), 0);
    ASSERT_FALSE(zombie.isAlive());
}

// Тест 3: Перевірка отримання точної летальної damage
TEST(CombatLogic, EntityDiesFromExactFatalDamage) {
    Zombie zombie("Target", 50, 10, 1, 2);
    zombie.takeDamage(50);

    ASSERT_EQ(zombie.getHealth(), 0);
    ASSERT_FALSE(zombie.isAlive());
}

// Тест 4: Перевірка, що health не стає негативним
TEST(CombatLogic, HealthDoesNotBecomeNegative) {
    Zombie zombie("Target", 10, 10, 1, 2);
    zombie.takeDamage(100);

    ASSERT_EQ(zombie.getHealth(), 0);
}

// Тест 5: Перевірка атаки Boss з урахуванням rage
TEST(CombatLogic, BossDealsCorrectDamageWithRage) {
    Player player("Hero", 100, 10, 1, 1);
    Boss boss("BigBoss", 200, 30, 15, 1, 2);
    int expectedHealth = 100 - (30 + 15); // 100 - 45 = 55

    boss.attack(player);

    ASSERT_EQ(player.getHealth(), expectedHealth);
}

// Тест 6: Перевірка атаки Zombie без rage
TEST(CombatLogic, ZombieDealsCorrectDamage) {
    Player player("Hero", 100, 10, 1, 1);
    Zombie zombie("Walker", 50, 10, 1, 2);
    int expectedHealth = 100 - 10; // 100 - 10 = 90

    zombie.attack(player);

    ASSERT_EQ(player.getHealth(), expectedHealth);
}

// Тест 7: Перевірка вибору другої weapon (gun)
TEST(PlayerLogic, PlayerChoosesGun) {
    Player player("Hero", 100, 20, 1, 1);
    player.chooseWeapon(2); // Пістолет, шкода 25
    Zombie zombie("Target", 80, 10, 1, 2);
    int expectedHealth = 80 - (20 + 25); // 80 - 45 = 35

    player.attack(zombie);

    ASSERT_EQ(zombie.getHealth(), expectedHealth);
}

// Тест 8: Перевірка вибору weapon за замовчуванням при невірному вводі
TEST(PlayerLogic, PlayerGetsDefaultWeaponOnInvalidChoice) {
    Player player("Hero", 100, 20, 1, 1);
    player.chooseWeapon(99);
    Zombie zombie("Target", 80, 10, 1, 2);
    int expectedHealth = 80 - (20 + 40); // 80 - 60 = 20

    player.attack(zombie);

    ASSERT_EQ(zombie.getHealth(), expectedHealth);
}

// Тест 9: Перевірка додавання очок for player
TEST(PlayerLogic, ScoreIncreasesCorrectly) {
    Player player("Hero", 100, 20, 1, 1);
    ASSERT_EQ(player.getScore(), 0);
    player.addScore(50);
    ASSERT_EQ(player.getScore(), 50);
    player.addScore(30);
    ASSERT_EQ(player.getScore(), 80);
}

// Тест 10: Перевірка коректного переміщення player на порожню клітинку
TEST(MovementLogic, PlayerMovesToEmptyCell) {
    Player player("Hero", 100, 20, 1, 1);
    Map map(5, 5, 0);

    player.move(1, 0, map.getGrid());

    ASSERT_EQ(player.getX(), 2);
    ASSERT_EQ(player.getY(), 1);
}

// Тест 11: Перевірка, що player не може рухатись у стіну
TEST(MovementLogic, PlayerCannotMoveIntoWall) {
    Player player("Hero", 100, 20, 1, 1);
    vector<vector<int>> grid = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };

    player.move(1, 0, grid);

    ASSERT_EQ(player.getX(), 1);
    ASSERT_EQ(player.getY(), 1);
}

// Тест 12: Перевірка, що player не може вийти за межі map вліво/вгору
TEST(MovementLogic, PlayerStaysWithinMapBoundsNegative) {
    Player player("Hero", 100, 20, 0, 0);
    Map map(5, 5, 0);

    player.move(-1, 0, map.getGrid());
    ASSERT_EQ(player.getX(), 0);
    ASSERT_EQ(player.getY(), 0);

    player.move(0, -1, map.getGrid());
    ASSERT_EQ(player.getX(), 0);
    ASSERT_EQ(player.getY(), 0);
}

// Тест 13: Перевірка, що player не може вийти за межі map вправо/вниз
TEST(MovementLogic, PlayerStaysWithinMapBoundsPositive) {
    Map map(5, 5, 0);
    Player player("Hero", 100, 20, 4, 4);

    player.move(1, 0, map.getGrid());
    ASSERT_EQ(player.getX(), 4);
    ASSERT_EQ(player.getY(), 4);

    player.move(0, 1, map.getGrid());
    ASSERT_EQ(player.getX(), 4);
    ASSERT_EQ(player.getY(), 4);
}

// Тест 14: Перевірка роботи контейнера: додавання та розмір
TEST(ContainerLogic, AddAndSize) {
    Container<Entity> container;
    ASSERT_EQ(container.size(), 0);
    container.add(make_unique<Zombie>("Z1", 10, 1, 0, 0));
    ASSERT_EQ(container.size(), 1);
    container.add(make_unique<Boss>("B1", 100, 10, 5, 0, 0));
    ASSERT_EQ(container.size(), 2);
}

// Тест 15: Перевірка роботи контейнера: отримання та видалення
TEST(ContainerLogic, GetAndRemove) {
    Container<Entity> container;
    container.add(make_unique<Zombie>("Z1", 10, 1, 0, 0));
    container.add(make_unique<Boss>("B1", 100, 10, 5, 0, 0));

    Entity* first = container.get(0);
    ASSERT_NE(first, nullptr);
    ASSERT_EQ(first->getName(), "Z1");

    Entity* second = container.get(1);
    ASSERT_NE(second, nullptr);
    ASSERT_EQ(second->getName(), "B1");

    Entity* outOfBounds = container.get(2);
    ASSERT_EQ(outOfBounds, nullptr);

    container.remove(0);
    ASSERT_EQ(container.size(), 1);
    Entity* nowFirst = container.get(0);
    ASSERT_NE(nowFirst, nullptr);
    ASSERT_EQ(nowFirst->getName(), "B1");
}

// Тест 16: Гравець намагається атакувати без зброї
TEST(CombatLogic, PlayerAttacksWithoutWeapon) {
    Player player("Hero", 100, 20, 1, 1);
    Zombie zombie("Target", 80, 10, 1, 2);
    int initialZombieHealth = zombie.getHealth();

    testing::internal::CaptureStdout();
    player.attack(zombie);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_EQ(zombie.getHealth(), initialZombieHealth);
    // Перевіряємо, що атака не пройшла (логіка залежить від реалізації attack, тут перевіряємо HP)
}

// Тест 17: Перевірка, що здоров'я не зменшується після атаки мертвого зомбі
TEST(CombatLogic, DeadZombieCannotAttack) {
    Player player("Hero", 100, 10, 1, 1);
    Zombie zombie("Walker", 0, 10, 1, 2);
    zombie.takeDamage(1); // Гарантуємо смерть
    ASSERT_FALSE(zombie.isAlive());
    int initialPlayerHealth = player.getHealth();

    zombie.attack(player);

    ASSERT_EQ(player.getHealth(), initialPlayerHealth);
}

// Тест 18: Спроба видалити елемент з порожнього контейнера
TEST(ContainerLogic, RemoveFromEmptyContainer) {
    Container<Entity> container;
    ASSERT_EQ(container.size(), 0);
    ASSERT_NO_THROW(container.remove(0));
    ASSERT_EQ(container.size(), 0);
}

// Тест 19: Спроба видалити елемент за невірним індексом
TEST(ContainerLogic, RemoveInvalidIndex) {
    Container<Entity> container;
    container.add(std::make_unique<Zombie>());
    ASSERT_EQ(container.size(), 1);

    ASSERT_NO_THROW(container.remove(1));
    ASSERT_EQ(container.size(), 1);

    ASSERT_NO_THROW(container.remove(-1));
    ASSERT_EQ(container.size(), 1);
}

// Тест 20: Спроба отримати елемент з порожнього контейнера
TEST(ContainerLogic, GetFromEmptyContainer) {
    Container<Entity> container;
    ASSERT_EQ(container.get(0), nullptr);
}

// Тест 21: Перевірка завантаження відсутнього файлу локалізації
TEST(LocalizationManager, LoadMissingFile) {
    LocalizationManager& lm = LocalizationManager::getInstance();
    bool result = lm.loadLanguage("non_existent_language_code");
    ASSERT_FALSE(result);
}

// Тест 22: Перевірка отримання відсутнього ключа локалізації
TEST(LocalizationManager, GetMissingKey) {
    LocalizationManager& lm = LocalizationManager::getInstance();
    lm.loadLanguage("en");
    std::string result = lm.getString("this_key_does_not_exist");
    ASSERT_EQ(result, "!!this_key_does_not_exist!!");
}

// Тест 23: Перевірка завантаження пошкодженого JSON файлу
TEST(LocalizationManager, LoadMalformedJsonFile) {
    LocalizationManager& lm = LocalizationManager::getInstance();
    const char* filename = "malformed_test.json";
    {
        std::ofstream tempFile(filename);
        tempFile << "{ \"key\": \"value\", ";
    }

    // Перехоплюємо stderr, бо туди пише LOG_ERR
    testing::internal::CaptureStderr();
    bool result = lm.loadLanguage("malformed_test");
    std::string output = testing::internal::GetCapturedStderr();

    ASSERT_FALSE(result);
    // Можна також перевірити output на наявність повідомлення про помилку
    std::remove(filename);
}

// --- НОВІ ТЕСТИ (AI, PLAYER MECHANICS, INVENTORY) ---

// Тест 24: Зомбі рухається до гравця по горизонталі
TEST(ZombieAI, MovesTowardsPlayerHorizontally) {
    vector<vector<int>> grid(5, vector<int>(5, 0));
    Zombie zombie("Walker", 50, 10, 1, 1);
    vector<Entity*> enemies;

    // Игрок справа на (4, 1)
    zombie.moveTowards(4, 1, grid, enemies);

    ASSERT_EQ(zombie.getX(), 2); // 1 -> 2
    ASSERT_EQ(zombie.getY(), 1);
}

// Тест 25: Зомбі рухається до гравця по вертикалі
TEST(ZombieAI, MovesTowardsPlayerVertically) {
    vector<vector<int>> grid(5, vector<int>(5, 0));
    Zombie zombie("Walker", 50, 10, 1, 1);
    vector<Entity*> enemies;

    // Игрок снизу на (1, 4)
    zombie.moveTowards(1, 4, grid, enemies);

    ASSERT_EQ(zombie.getX(), 1);
    ASSERT_EQ(zombie.getY(), 2); // 1 -> 2
}

// Тест 26: Зомбі обходить стіну
TEST(ZombieAI, ZombieAvoidsWall) {
    vector<vector<int>> grid(5, vector<int>(5, 0));
    grid[1][2] = 1; // Стіна справа від зомбі (який на 1,1)
    Zombie zombie("Walker", 50, 10, 1, 1);
    vector<Entity*> enemies;

    // Гравець справа за стіною (3, 1)
    zombie.moveTowards(3, 1, grid, enemies);

    // Зомбі не повинен увійти в стіну (2,1)
    bool safePosition = (zombie.getX() != 2 || zombie.getY() != 1);
    ASSERT_TRUE(safePosition);
    // Перевіряємо, що він все ще на карті
    ASSERT_TRUE(zombie.getX() >= 0 && zombie.getX() < 5);
}

// Тест 27: Зомбі не накладаються один на одного
TEST(ZombieAI, ZombieBlockedByAnotherZombie) {
    vector<vector<int>> grid(5, vector<int>(5, 0));
    Zombie z1("Z1", 50, 10, 1, 1);
    Zombie z2("Z2", 50, 10, 2, 1); // Стоїть на шляху Z1 праворуч
    vector<Entity*> enemies = { &z1, &z2 };

    // Z1 намагається йти до (4, 1), шлях перекрито Z2
    z1.moveTowards(4, 1, grid, enemies);

    ASSERT_EQ(z1.getX(), 1); // Має залишитися на місці або піти в обхід, але не на (2,1)
    ASSERT_NE(z1.getX(), 2);
}

// Тест 28: Лікування гравця не перевищує максимум
TEST(PlayerLogic, HealDoesNotExceedMaxHealth) {
    Player player("Hero", 50, 10, 0, 0); // Start HP 50
    player.heal(20);
    ASSERT_EQ(player.getHealth(), 70);

    player.heal(100); // Лікуємо понад макс (100)
    ASSERT_EQ(player.getHealth(), 100);
}

// Тест 29: Збір патронів
TEST(PlayerLogic, AmmoCollection) {
    Player player("Hero", 100, 10, 0, 0);
    int initialAmmo = player.getAmmo(); // Зазвичай 10

    player.addAmmo(5);
    ASSERT_EQ(player.getAmmo(), initialAmmo + 5);
}

// Тест 30: Неможливість стріляти без патронів
TEST(CombatLogic, CannotAttackWithGunAndNoAmmo) {
    Player player("Hero", 100, 10, 0, 0);
    player.chooseWeapon(2); // Gun

    // Витрачаємо всі патрони
    int ammo = player.getAmmo();
    for(int i = 0; i < ammo; ++i) {
        Zombie dummy("Target", 1000, 0, 0, 0);
        player.attack(dummy);
    }
    ASSERT_EQ(player.getAmmo(), 0);
    ASSERT_FALSE(player.canAttack());

    // Пробуємо атакувати
    Zombie z("Target", 100, 10, 0, 0);
    int hpBefore = z.getHealth();
    player.attack(z);
    ASSERT_EQ(z.getHealth(), hpBefore);
}

// Тест 31: Зміна зброї (Swap)
TEST(PlayerLogic, SwapWeaponTogglesType) {
    Player player("Hero", 100, 10, 0, 0);
    player.chooseWeapon(1); // Sword
    ASSERT_EQ(player.getWeaponName(), "Sword");

    player.swapWeapon();
    ASSERT_EQ(player.getWeaponName(), "Gun");

    player.swapWeapon();
    ASSERT_EQ(player.getWeaponName(), "Sword");
}

// Тест 32: Базові тести класу Inventory (незалежний тест)
TEST(InventoryLogic, AddAndRetrieveItem) {
    Inventory inv;
    Sword* sword = new Sword();
    inv.addItem(sword);

    ASSERT_NE(inv.getItem(0), nullptr);
    ASSERT_EQ(inv.getItem(0)->getName(), "Sword");
    ASSERT_EQ(inv.getItem(1), nullptr); // За межами
}

// Тест 33: Очищення тайла на карті
TEST(MapLogic, ClearTileRemovesItem) {
    Map map(5, 5, 0);
    // Припустимо, що на карті є об'єкти, але ми точно знаємо, що clearTile робить 0
    map.clearTile(1, 1);
    ASSERT_EQ(map.getGrid()[1][1], 0); // 0 == TILE_FLOOR
}

// Тест 34: Перевірка властивостей зброї
TEST(WeaponLogic, WeaponProperties) {
    Gun gun;
    ASSERT_TRUE(gun.isRanged());
    ASSERT_GT(gun.getRange(), 1); // Range > 1

    Sword sword;
    ASSERT_FALSE(sword.isRanged());
    ASSERT_EQ(sword.getRange(), 1); // Melee range is usually 1 (or 0 depending on logic, assumed 1)
}