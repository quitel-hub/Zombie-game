#pragma once
#include <vector>
#include <memory> // Для unique_ptr
using namespace std;

#ifndef UNTITLED23_CONTAINER_H
#define UNTITLED23_CONTAINER_H

#endif //UNTITLED23_CONTAINER_H

/**
 * @brief Шаблонний клас-контейнер для поліморфного зберігання об'єктів.
 *
 * Використовує std::vector із std::unique_ptr для керування пам'яттю
 * та володінням об'єктами (наприклад, ворогами типу Entity).
 * @tparam T Базовий тип об'єктів, що зберігаються (наприклад, Entity).
 */
template<typename T>
class Container {
    vector<std::unique_ptr<T>> items; ///< Вектор розумних вказівників

public:
    Container() = default;

    // --- Заборона копіювання (оскільки unique_ptr не можна копіювати) ---
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    // --- Дозвіл переміщення ---
    Container(Container&&) = default;
    Container& operator=(Container&&) = default;

    ~Container() = default;

    /**
     * @brief Очищує контейнер, видаляючи всі елементи.
     */
    void clear() {
        items.clear();
    }

    /**
     * @brief Додає новий елемент у контейнер.
     * @param item `unique_ptr` на об'єкт, що передається у володіння контейнеру.
     */
    void add(unique_ptr<T> item) {
        items.push_back(move(item));
    }

    /**
     * @brief Отримує "сирий" вказівник на елемент за індексом.
     * @param index Індекс елемента.
     * @return "Сирий" вказівник на T, або nullptr, якщо індекс недійсний.
     */
    T* get(size_t index) const {
        if (index < items.size()) {
            return items[index].get();
        }
        return nullptr;
    }

    /**
     * @brief Повертає вектор "сирих" вказівників на всі елементи.
     *
     * Корисно для передачі списку об'єктів у функції, що не керують пам'яттю.
     * @return std::vector<T*> вектор вказівників.
     */
    vector<T*> getAllRaw() const {
        vector<T*> raw_pointers;
        raw_pointers.reserve(items.size());
        for (const auto& ptr : items) {
            raw_pointers.push_back(ptr.get());
        }
        return raw_pointers;
    }

    /**
     * @brief Повертає кількість елементів у контейнері.
     * @return size_t кількість елементів.
     */
    size_t size() const { return items.size(); }

    /**
     * @brief Видаляє елемент за індексом.
     * @param index Індекс елемента для видалення.
     */
    void remove(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }
};