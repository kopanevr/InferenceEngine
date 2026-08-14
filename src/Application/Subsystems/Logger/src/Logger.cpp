#include "Logger.hpp"

//

#include "SubsystemId.hpp"

//

/// @brief Конструктор.
Logger::Logger()
{
    // Инициализация.

    init();
}

/// @brief Инициализация подсистемы.
void Logger::init()
{
    subsystemHandle.id   = SubsystemId::Logger;
    subsystemHandle.name = "Logger";
}
