#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "json.hpp"
#include "Logger.h"
#include <SFML/System/String.hpp>

#ifndef UNTITLED23_LOCALIZATIONMANAGER_H
#define L10N LocalizationManager::getInstance()
using namespace nlohmann;
using namespace std;
#endif
/**
 * @brief Менеджер локалізації (Singleton).
 */

class LocalizationManager {
private:
    nlohmann::json translations;
    LocalizationManager() {}

public:
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
            file.clear();
            file.open("assets/" + filename);
        }
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


    sf::String getString(const string& key) {
        string utf8Str = "!!" + key + "!!";

        if (translations.contains(key)) {
            utf8Str = translations[key].get<string>();
        }


        return sf::String::fromUtf8(utf8Str.begin(), utf8Str.end());
    }


    template<typename... Args>
    sf::String getFormattedString(const string& key, const Args&... args) {
        string format_str;
        if (translations.contains(key)) {
            format_str = translations[key].get<string>();
        } else {
            return sf::String("!!" + key + "!!");
        }

        vector<std::string> string_args;
        ( (string_args.push_back( (stringstream{} << args).str() )), ...);


        for (size_t i = 0; i < string_args.size(); ++i) {
            string placeholder = "{" + to_string(i) + "}";
            size_t pos = format_str.find(placeholder);
            while (pos != string::npos) {
                format_str.replace(pos, placeholder.length(), string_args[i]);
                pos = format_str.find(placeholder, pos + string_args[i].length());
            }
        }


        return sf::String::fromUtf8(format_str.begin(), format_str.end());
    }
};