#pragma once
#include <vector>
#include <memory> // Для unique_ptr
using namespace std;

#ifndef UNTITLED23_CONTAINER_H
#define UNTITLED23_CONTAINER_H

#endif //UNTITLED23_CONTAINER_H

/**
 * @brief Універсальний контейнер для зберігання об'єктів.
 * @tparam T Тип об'єктів, що зберігаються (зазвичай Entity або його спадкоємці).
 * @details Використовує std::vector та std::unique_ptr для автоматичного управління пам'яттю.
 */
template<typename T>
class Container {
    vector<std::unique_ptr<T>> items;///< Вектор розумних вказівників на об'єкти

public:
    /**
     * @brief Додає новий об'єкт у контейнер.
     * @param item Унікальний вказівник на об'єкт.
     */
    Container() = default;


    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;


    Container(Container&&) = default;
    Container& operator=(Container&&) = default;

    ~Container() = default;

    /**
     * @brief Очищує контейнер, видаляючи всі елементи.
     */
    void clear() {
        items.clear();
    }


    void add(unique_ptr<T> item) {
        items.push_back(move(item));
    }

    /**
     * @brief Отримує доступ до об'єкта за індексом.
     * @param index Індекс елемента.
     * @return Вказівник на об'єкт або nullptr, якщо індекс некоректний.
     */
    T* get(size_t index) const {
        if (index < items.size()) {
            return items[index].get();
        }
        return nullptr;
    }

    /**
     * @brief Отримує вектор "сирих" вказівників для ітерації.
     * @details Корисно для циклів `for (auto* e : container.getAllRaw())`.
     * @return Вектор звичайних вказівників T*.
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
     */
    size_t size() const { return items.size(); }

    /**
     * @brief Видаляє об'єкт з контейнера за індексом.
     * @param index Індекс елемента для видалення.
     */
    void remove(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }
};