#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "json.hpp" // nlohmann/json

#ifndef UNTITLED23_LOCALIZATIONMANAGER_H
/**
 * @def L10N
 * @brief Макрос для швидкого доступу до сінглтону LocalizationManager.
 * Використовуйте L10N.getString("key") або L10N.getFormattedString("key", args...).
 */
#define L10N LocalizationManager::getInstance()
using namespace nlohmann; // Для json
using namespace std;
#endif //UNTITLED23_LOCALIZATIONMANAGER_H

/**
 * @brief Клас-сінглтон для керування локалізацією.
 *
 * Завантажує рядки тексту з JSON-файлів (наприклад, en.json, ukr.json)
 * та надає доступ до них за ключем.
 * Використовує nlohmann::json для парсингу.
 */
class LocalizationManager {
private:
    nlohmann::json translations; ///< Об'єкт JSON, що зберігає всі рядки.

    /**
     * @brief Приватний конструктор (для реалізації Сінглтона).
     */
    LocalizationManager() {}

public:
    // --- Заборона копіювання та присвоєння (Сінглтон) ---
    LocalizationManager(const LocalizationManager&) = delete;
    LocalizationManager& operator=(const LocalizationManager&) = delete;

    /**
     * @brief Статичний метод для отримання єдиного екземпляра класу.
     * @return LocalizationManager& посилання на екземпляр.
     */
    static LocalizationManager& getInstance() {
        static LocalizationManager instance;
        return instance;
    }

    /**
     * @brief Завантажує файл мови у форматі JSON.
     * @param lang_code Код мови (наприклад, "en", "ukr"), що є назвою файлу.
     * @return true у разі успішного завантаження та парсингу, інакше false.
     */
    bool loadLanguage(const string& lang_code) {
        string filename = lang_code + ".json";
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open language file " << filename << endl;
            return false;
        }
        try {
            translations = nlohmann::json::parse(file);
        } catch (json::parse_error& e) {
            cerr << "Error parsing JSON: " << e.what() << endl;
            return false;
        }
        return true;
    }

    /**
     * @brief Отримує рядок тексту за ключем.
     * @param key Ключ, що відповідає рядку в JSON.
     * @return Рядок тексту або "!!key!!" у разі помилки.
     */
    string getString(const string& key) {
        if (translations.contains(key)) {
            return translations[key].get<string>();
        }
        return "!!" + key + "!!"; // Повертає ключ, якщо переклад не знайдено
    }

    /**
     * @brief Отримує та форматує рядок (заміна {0}, {1} тощо).
     *
     * @example L10N.getFormattedString("player_hp", "PlayerName", 100);
     * // JSON: "player_hp": "{0} has {1} HP."
     * // Result: "PlayerName has 100 HP."
     *
     * @tparam Args Типи аргументів для форматування.
     * @param key Ключ рядка форматування.
     * @param args Аргументи для підстановки.
     * @return Відформатований рядок.
     */
    template<typename... Args>
    string getFormattedString(const string& key, const Args&... args) {
        string format_str = getString(key);

        // Конвертуємо всі аргументи в рядки
        vector<std::string> string_args;
        ( (string_args.push_back( (stringstream{} << args).str() )), ...);

        // Замінюємо плейсхолдери {0}, {1}, ...
        for (size_t i = 0; i < string_args.size(); ++i) {
            string placeholder = "{" + to_string(i) + "}";
            size_t pos = format_str.find(placeholder);
            if (pos != string::npos) {
                format_str.replace(pos, placeholder.length(), string_args[i]);
            }
        }
        return format_str;
    }
};