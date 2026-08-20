/**
 * @file
 * @brief Подсистема регистратора событий.
 */

#pragma once

//

//

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <utility>

//

#include "Subsystem.hpp"

//

/// @brief Регистратор событий.
class Logger final : public Subsystem
{
private:
    /// @brief Буфер для записи данных.
    std::vector<std::string> recordedDataBuffer;

    /// @brief
    mutable std::mutex mutex;
private:
    /// @brief Конструктор.
    Logger();

    Logger& operator=(const Logger&) = delete;
    Logger(const Logger&) = delete;

    /// @brief Инициализация подсистемы.
    void init() override;

    /// @brief Предварительная настройка перед запуском подсистемы.
    void setBeforeStartUp() override {}

    /// @brief Предварительная настройка перед остановкой подсистемы.
    void setBeforeShutDown() override {}

    /// @brief Тело процесса.
    int processBody() override { return 0; }
public:
    /// @brief Деструктор.
    ~Logger() = default;

    static Logger* getInstance() noexcept
    {
        static Logger instance = {};

        return &instance;
    }

    /// @brief
    /// @param args Данные для вывода.
    template<typename... Args>
    void log(Args&&... args) const noexcept
    {
        printToTerminal(std::forward<Args>(args)...);
    }

    /// @brief
    /// @brief Выводит данные в терминал.
    /// @param args Данные для вывода.
    template<typename... Args>
    void printToTerminal(Args&&... args) const noexcept
    {
        try
        {
            std::lock_guard<std::mutex> lock(mutex);

            ((std::cout << std::forward<Args>(args) << " "), ...);

            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            //
        }
    }

    /// @brief Записывает данные в буфер.
    /// @param bufferSize Размер буфера для записи данных.
    /// @param args Записываемые данные в буфер.
    template<size_t bufferSize = 5u, typename... Args>
    void recordDataToBuffer(Args... args) noexcept {}
};

//

#define LOG(...) Logger::getInstance()->log(__VA_ARGS__)

//

#ifndef NDEBUG
#   define DEBUG(...) LOG("[ОТЛАДКА]" __VA_OPT__(,) __VA_ARGS__)
#else
#   define DEBUG(...) ((void)0)
#endif

#define INFO(...) LOG("[ИНФО]" __VA_OPT__(,) __VA_ARGS__)
#define WARNING(...) LOG("[ВНИМАНИЕ]" __VA_OPT__(,) __VA_ARGS__)
#define ERROR(...) LOG("[ОШИБКА]" __VA_OPT__(,) __VA_ARGS__)

#define SEPARATOR LOG("------")
