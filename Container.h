#pragma once
#include <vector>
#include <memory> // Для unique_ptr
using namespace std;

#ifndef UNTITLED23_CONTAINER_H
#define UNTITLED23_CONTAINER_H

#endif //UNTITLED23_CONTAINER_H


template<typename T>
class Container {
    vector<std::unique_ptr<T>> items;

public:
    Container() = default;

    // --- Заборона копіювання (оскільки unique_ptr не можна копіювати) ---
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    // --- Дозвіл переміщення ---
    Container(Container&&) = default;
    Container& operator=(Container&&) = default;

    ~Container() = default;


    void clear() {
        items.clear();
    }


    void add(unique_ptr<T> item) {
        items.push_back(move(item));
    }


    T* get(size_t index) const {
        if (index < items.size()) {
            return items[index].get();
        }
        return nullptr;
    }


    vector<T*> getAllRaw() const {
        vector<T*> raw_pointers;
        raw_pointers.reserve(items.size());
        for (const auto& ptr : items) {
            raw_pointers.push_back(ptr.get());
        }
        return raw_pointers;
    }


    size_t size() const { return items.size(); }


    void remove(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }
};