#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Змінюємо назви (Info, Error...), щоб уникнути конфлікту з макросами Windows (INFO, ERROR...)
enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

class Logger {
private:
    std::ofstream logFile;
    std::mutex logMutex;

    Logger() {
        logFile.open("game_log.txt", std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            std::cerr << "[CRITICAL ERROR] Cannot open game_log.txt!" << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tmBuf;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tmBuf, &now);
#else
        localtime_r(&now, &tmBuf);
#endif

        std::string levelStr;
        std::string colorCode = "";
        std::string resetCode = "\033[0m";

        // Оновлюємо switch для нових назв
        switch(level) {
            case LogLevel::Info:
                levelStr = "[INFO]";
                colorCode = "\033[32m"; // Зелений
                break;
            case LogLevel::Warning:
                levelStr = "[WARN]";
                colorCode = "\033[33m"; // Жовтий
                break;
            case LogLevel::Error:
                levelStr = "[ERROR]";
                colorCode = "\033[31m"; // Червоний
                break;
            case LogLevel::Debug:
                levelStr = "[DEBUG]";
                colorCode = "\033[36m"; // Блакитний
                break;
        }

        std::stringstream ss;
        ss << "[" << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S") << "] "
           << levelStr << " " << message;

        std::string finalLog = ss.str();

        if (logFile.is_open()) {
            logFile << finalLog << std::endl;
        }

        std::cout << colorCode << finalLog << resetCode << std::endl;
    }
};

// Макроси залишаємо такими ж, але всередині викликаємо нові імена з enum
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::Info, msg)
#define LOG_WARN(msg) Logger::getInstance().log(LogLevel::Warning, msg)
#define LOG_ERR(msg)  Logger::getInstance().log(LogLevel::Error, msg)
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::Debug, msg)