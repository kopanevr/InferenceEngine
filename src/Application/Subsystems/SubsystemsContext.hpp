#pragma once

#include <condition_variable>

/// @brief
struct SubsystemContext final
{
    SubsystemContext* getInstance()
    {
        static SubsystemContext instance = {};

        return &instance;
    }
};
