#pragma once

//

#include <cstdint>

//

#include <chrono>

//

using TimerId = uint8_t;

//

/// @brief Таймер.
struct Timer final
{
    /// @brief Идентификатор таймера.
    TimerId id;

    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point end{};

    /// @brief
    bool isStarted : 1{false};
    /// @brief
    bool isDurationSet : 1{false};

    /// @brief Конструктор.
    constexpr explicit Timer(const TimerId timerId)
        : id{timerId}
    {}
};
