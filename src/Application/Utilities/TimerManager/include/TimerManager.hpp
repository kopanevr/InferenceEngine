/**
 * @file
 * @brief Содержит описание таймера.
 */

#pragma once

//

#include <chrono>

//

#include "../../Timer/include/Timer.hpp"

//

/// @brief Менеджер таймера.
class TimerManager final
{
public:
    TimerManager() = default;
    ~TimerManager() = default;

    /// @brief Запуск таймера.
    /// @details Запуск таймера без установки периода времени отсчета.
    /// @param timer Таймер.
    void start(Timer& timer)
    {
        timer.start = std::chrono::steady_clock::now();

        timer.isStarted = true;
        timer.isDurationSet = false;
    }

    /// @brief Запуск таймера.
    /// @details Запуск таймера c установкой периода времени отсчета.
    /// @param timer Таймер.
    /// @param duration Период времени.
    void start(Timer& timer, std::chrono::steady_clock::duration duration)
    {
        timer.start = std::chrono::steady_clock::now();
        timer.end = timer.start + duration;

        timer.isStarted = true;
        timer.isDurationSet = true;
    }

    /// @brief Остановка таймера.
    /// @param timer Таймер.
    void stop(Timer& timer)
    {
        if (timer.isStarted)
        {
            timer.end = std::chrono::steady_clock::now();

            timer.isStarted = false;
        }
    }

    /// @brief Проверка запуска таймера.
    /// @return Состояние запуска таймера.
    /// @param timer Таймер.
    [[nodiscard]] bool isStarted(const Timer& timer) const noexcept
    {
        return timer.isStarted;
    }

    /// @brief Проверка работы таймера.
    /// @return Состояние работы таймера.
    /// @param timer Таймер.
    [[nodiscard]] bool isRunning(const Timer& timer) const noexcept
    {
        return timer.isStarted && !isFinished(timer);
    }

    /// @brief Проверка завершения таймера.
    /// @return Состояние завершения таймера.
    /// @param timer Таймер.
    [[nodiscard]] bool isFinished(const Timer& timer) const noexcept
    {
        if (!timer.isStarted) { return true; }

        // При отсутствии периода времени отсчета.

        if (!timer.isDurationSet) { return false; }

        return std::chrono::steady_clock::now() >= timer.end;
    }

    /// @brief Проверка остановки таймера.
    /// @return Состояние остановки таймера.
    /// @param timer Таймер.
    [[nodiscard]] bool isStopped(const Timer& timer) const noexcept
    {
        return !timer.isStarted;
    }

    /// @brief Возвращает истекший период времени.
    /// @return Истекший период времени.
    /// @param timer Таймер.
    [[nodiscard]] std::chrono::steady_clock::duration getElapsedTime(const Timer& timer) const noexcept
    {
        if (timer.isStarted)
        {
            return timer.end - timer.start;
        }

        return std::chrono::steady_clock::now() - timer.start;
    }
};
