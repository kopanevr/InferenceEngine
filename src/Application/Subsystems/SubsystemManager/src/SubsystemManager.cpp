#include "SubsystemManager.hpp"

//

SubsystemManager* SubsystemManager::instance = {};

/// @brief Инициализация подсистемы.
void SubsystemManager::init()
{
    subsystemHandle.id   = SubsystemId::SubsystemManager;
    subsystemHandle.name = "Manager";
}
