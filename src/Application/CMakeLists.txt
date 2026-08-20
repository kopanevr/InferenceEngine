add_executable(
    Application
    Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineInterpreter/src/CommandLineInterpreter.cpp
    main.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/FrameCaptureSubsystem)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/InferenceSubsystem)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/SubsystemManager)

target_include_directories(
    Application PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationConfig/include
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineInterpreter/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TimerManager/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Subsystems/Subsystem/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Subsystems/InferenceSubsystem/include
)

target_link_libraries(
    Application PRIVATE
    FrameCaptureSubsystem
    InferenceSubsystem
    SubsystemManager
)

add_custom_command(
    TARGET Application
    POST_BUILD
    COMMAND size Application --format=gnu
    COMMENT "Размеры секций:"
)
