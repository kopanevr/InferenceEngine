add_library(
    FrameCaptureSubsystem SHARED
    src/FrameCaptureSubsystem.cpp
)

target_include_directories(
    FrameCaptureSubsystem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
    ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
    ${CMAKE_SOURCE_DIR}/src/ApplicationContext/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/BitField/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/TimerManager/include
)

target_link_libraries(FrameCaptureSubsystem PUBLIC Logger)
