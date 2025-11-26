#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "json.hpp"
#include "Logger.h"

#ifndef UNTITLED23_LOCALIZATIONMANAGER_H
#define L10N LocalizationManager::getInstance()
using namespace nlohmann;
using namespace std;
#endif
/**
 * @brief Менеджер локалізації (Singleton).
 * @details Відповідає за завантаження текстових рядків з JSON файлів
 * та їх підстановку у гру.
 */
class LocalizationManager {
private:
    nlohmann::json translations; ///< Зберігає завантажені переклади
    LocalizationManager() {}

public:
    LocalizationManager(const LocalizationManager&) = delete;
    LocalizationManager& operator=(const LocalizationManager&) = delete;
    /**
     * @brief Повертає єдиний екземпляр менеджера.
     */
    static LocalizationManager& getInstance() {
        static LocalizationManager instance;
        return instance;
    }
    /**
     * @brief Завантажує мовний файл.
     * @param lang_code Код мови (наприклад, "ukr", "en").
     * @return true, якщо завантаження успішне.
     */
    bool loadLanguage(const string& lang_code) {
        string filename = lang_code + ".json";
        // Спробуємо знайти файл поруч з exe або в assets
        ifstream file(filename);
        if (!file.is_open()) {

            LOG_ERR("Could not open language file: " + filename);
            return false;
        }
        try {
            translations = nlohmann::json::parse(file);
        } catch (json::parse_error& e) {

            LOG_ERR("Error parsing JSON: " + string(e.what()));
            return false;
        }

        LOG_INFO("Language loaded: " + lang_code);
        return true;
    }
    /**
     * @brief Отримує рядок за ключем.
     * @param key Ключ у JSON файлі.
     * @return Перекладений рядок або ключ із знаками оклику, якщо не знайдено.
     */
    string getString(const string& key) {
        if (translations.contains(key)) {
            return translations[key].get<string>();
        }
        return "!!" + key + "!!";
    }
    /**
     * @brief Отримує відформатований рядок із підстановкою аргументів.
     * @details Замінює плейсхолдери {0}, {1} і т.д. на передані аргументи.
     * @tparam Args Типи аргументів.
     * @param key Ключ локалізації.
     * @param args Список значень для підстановки.
     * @return Готовий рядок.
     */
    template<typename... Args>
    string getFormattedString(const string& key, const Args&... args) {
        string format_str = getString(key);
        vector<std::string> string_args;
        ( (string_args.push_back( (stringstream{} << args).str() )), ...);

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