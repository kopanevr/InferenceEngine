#pragma once

//

#include <cstdint>

//

/// @brief Идентификаторы подсистем.
enum class SubsystemId : uint8_t
{
    SubsystemManager,

    InferenceSubsystem,
    Logger,

    Count
};
