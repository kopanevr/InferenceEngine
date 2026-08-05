#pragma once

//

#include "Subsystem.hpp"

//

/// @brief
class Logger final : public Subsystem
{
private:
    /// @brief Конструктор.
    Logger();
    /// @brief Деструктор.
    ~Logger();
    Logger& operator=(const Logger&) = delete;
    Logger(const Logger&) = delete;

    /// @brief Инициализация подсистемы.
    void init() override;

    /// @brief Предварительная настройка перед запуском подсистемы.
    void setBeforeStartUp() override;

    /// @brief Предварительная настройка перед остановкой подсистемы.
    void setBeforeShutDown() override;

    /// @brief Тело процесса.
    int processBody() override;
public:
    static Logger* getInstance()
    {
        static Logger instance = {};

        return &instance;
    }
};
