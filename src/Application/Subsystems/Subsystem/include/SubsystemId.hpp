#pragma once

//

#include <cstdint>

//

/// @brief Идентификаторы подсистем.
enum class SubsystemId : uint8_t
{
    SubsystemManager,

    //

    FrameCaptureSubsystem,
    InferenceSubsystem,
    Logger,

    //

    Count
};
