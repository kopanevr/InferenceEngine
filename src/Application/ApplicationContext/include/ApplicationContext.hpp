#pragma once

#include <condition_variable>

/// @brief
struct ApplicationContext final
{
    ApplicationContext* getInstance()
    {
        static ApplicationContext instance = {};

        return &instance;
    }
};
