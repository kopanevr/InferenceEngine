add_library(Logger SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp)

target_include_directories(
    Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/BitField/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/TimerManager/include
)
