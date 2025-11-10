#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "json.hpp" // nlohmann/json

#ifndef UNTITLED23_LOCALIZATIONMANAGER_H

#define L10N LocalizationManager::getInstance()
using namespace nlohmann; // Для json
using namespace std;
#endif //UNTITLED23_LOCALIZATIONMANAGER_H


class LocalizationManager {
private:
    nlohmann::json translations;

    LocalizationManager() {}

public:
    // --- Заборона копіювання та присвоєння (Сінглтон) ---
    LocalizationManager(const LocalizationManager&) = delete;
    LocalizationManager& operator=(const LocalizationManager&) = delete;


    static LocalizationManager& getInstance() {
        static LocalizationManager instance;
        return instance;
    }


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


    string getString(const string& key) {
        if (translations.contains(key)) {
            return translations[key].get<string>();
        }
        return "!!" + key + "!!"; // Повертає ключ, якщо переклад не знайдено
    }


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