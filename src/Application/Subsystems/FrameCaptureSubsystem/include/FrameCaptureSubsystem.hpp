/**
 * @file
 * @brief Подсистема захвата кадра.
 */

#pragma once

//

#include <cstdint>

//

#include <thread>

//

#include "Subsystem.hpp"

//

/// @brief
class FrameCaptureSubsystem final : public Subsystem
{
private:
    /// @brief Поток захвата.
    std::thread captureThread;
private:
    /// @brief Конструктор.
    FrameCaptureSubsystem();

    FrameCaptureSubsystem& operator=(const FrameCaptureSubsystem&) = delete;
    FrameCaptureSubsystem(const FrameCaptureSubsystem&) = delete;

    /// @brief Инициализация подсистемы.
    void init() override;

    /// @brief Предварительная настройка перед запуском подсистемы.
    void setBeforeStartUp() override;

    /// @brief Предварительная настройка перед остановкой подсистемы.
    void setBeforeShutDown() override;

    /// @brief Тело процесса.
    int processBody() override;

    /// @brief
    void run();

    /// @brief
    bool body();
public:
    /// @brief Деструктор.
    ~FrameCaptureSubsystem() = default;

    static FrameCaptureSubsystem* getInstance() noexcept
    {
        static FrameCaptureSubsystem instance = {};

        return &instance;
    }
};
