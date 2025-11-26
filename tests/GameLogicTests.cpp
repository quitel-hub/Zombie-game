#include "gtest/gtest.h"
#include "../Player.h"
#include "../Zombie.h"
#include "../Boss.h"
#include "../Map.h"
#include "../Container.h"
#include <vector>

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

    // Assert
    ASSERT_EQ(zombie.getHealth(), initialZombieHealth);
    ASSERT_NE(output.find("has no weapon equipped"), std::string::npos);
}

// Тест 17: Перевірка, що здоров'я не зменшується після атаки мертвого зомбі
TEST(CombatLogic, DeadZombieCannotAttack) {
    Player player("Hero", 100, 10, 1, 1);
    Zombie zombie("Walker", 0, 10, 1, 2);
    zombie.takeDamage(1);
    ASSERT_FALSE(zombie.isAlive());
    int initialPlayerHealth = player.getHealth();


    zombie.attack(player);

    // Assert
    ASSERT_EQ(player.getHealth(), initialPlayerHealth);
}

// Тест 17: Спроба видалити елемент з порожнього контейнера
TEST(ContainerLogic, RemoveFromEmptyContainer) {
    Container<Entity> container;
    ASSERT_EQ(container.size(), 0);

    // Act & Assert
    ASSERT_NO_THROW(container.remove(0));
    ASSERT_EQ(container.size(), 0);
}

// Тест 18: Спроба видалити елемент за невірним індексом
TEST(ContainerLogic, RemoveInvalidIndex) {
    Container<Entity> container;
    container.add(std::make_unique<Zombie>());
    ASSERT_EQ(container.size(), 1);


    ASSERT_NO_THROW(container.remove(1));
    ASSERT_EQ(container.size(), 1);


    ASSERT_NO_THROW(container.remove(-1));
    ASSERT_EQ(container.size(), 1);
}

// Тест 19: Спроба отримати елемент з порожнього контейнера
TEST(ContainerLogic, GetFromEmptyContainer) {
    Container<Entity> container;
    ASSERT_EQ(container.get(0), nullptr);
}

// Тест 20: Перевірка завантаження відсутнього файлу локалізації
TEST(LocalizationManager, LoadMissingFile) {
    LocalizationManager& lm = LocalizationManager::getInstance();

    // Act
    bool result = lm.loadLanguage("non_existent_language_code");

    // Assert
    ASSERT_FALSE(result);

}

// Тест 21: Перевірка отримання відсутнього ключа локалізації
TEST(LocalizationManager, GetMissingKey) {
    LocalizationManager& lm = LocalizationManager::getInstance();
    lm.loadLanguage("en");

    // Act
    std::string result = lm.getString("this_key_does_not_exist");

    // Assert
    ASSERT_EQ(result, "!!this_key_does_not_exist!!");
}

// Тест 22: Перевірка завантаження пошкодженого JSON файлу
TEST(LocalizationManager, LoadMalformedJsonFile) {
    LocalizationManager& lm = LocalizationManager::getInstance();
    const char* filename = "malformed_test.json";

    // Створюємо тимчасовий пошкоджений JSON файл
    {
        std::ofstream tempFile(filename);
        tempFile << "{ \"key\": \"value\", ";
    }

    // Act
    testing::internal::CaptureStderr();
    bool result = lm.loadLanguage("malformed_test");
    std::string output = testing::internal::GetCapturedStderr();

    // Assert
    ASSERT_FALSE(result);
    ASSERT_NE(output.find("Error parsing JSON"), std::string::npos);


    std::remove(filename);
}
