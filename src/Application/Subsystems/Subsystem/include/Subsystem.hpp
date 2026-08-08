/**
 * @file
 * @brief Содержит описание подсистемы приложения.
 */

#pragma once

//

#include <cstdint>

//

#include <string>

//

#include "SubsystemId.hpp"

//

struct SubsystemHandle
{
    /// @brief Идентификатор подсистемы.
    SubsystemId id;

    /// @brief Имя подсистемы.
    std::string name;

    /// @brief Состояние запуска подсистемы.
    bool isStarted : 1;
};

/// @brief Подсистема.
class Subsystem
{
private:
protected:
    /// @brief Дескриптор подсистемы.
    SubsystemHandle subsystemHandle;
protected:
    /// @brief Инициализация подсистемы.
    virtual void init() = 0;

    /// @brief
    virtual void setBeforeStartUp() = 0;

    /// @brief
    virtual void setBeforeShutDown() = 0;

    /// @brief Тело основного цикла.
    virtual int processBody() = 0;
public:
    /// @brief Конструктор.
    Subsystem() = default;

    /// @brief Деструктор.
    virtual ~Subsystem() = default;

    /// @brief Запуск подсистемы.
    void startUp()
    {
        if (subsystemHandle.isStarted) { return; }

        setBeforeStartUp();

        subsystemHandle.isStarted = true;
    }

    /// @brief Остановка подсистемы.
    void shutDown()
    {
        if (!subsystemHandle.isStarted) { return; }

        setBeforeShutDown();

        subsystemHandle.isStarted = false;
    }

    /// @brief Возвращает идентификатор подсистемы.
    [[nodiscard]] SubsystemId getId() const noexcept { return subsystemHandle.id; }

    /// @brief Проверка запуска подсистемы.
    [[nodiscard]] bool isRunning() const noexcept { return subsystemHandle.isStarted; }

    /// @brief Основной процесс.
    int process()
    {
        if (!subsystemHandle.isStarted) { return 1; }

        return processBody();
    }
};
