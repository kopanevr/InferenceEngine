add_executable(Application
    Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Application/CommandLineInterpreter/src/CommandLineInterpreter.cpp
    main.cpp
)

target_include_directories(Application PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Application
    ${CMAKE_SOURCE_DIR}/src/Application/ApplicationConfig/include
    ${CMAKE_SOURCE_DIR}/src/Application/CommandLineInterpreter/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/TimerManager/include
)

target_link_libraries(Application PRIVATE
    InferenceSubsystem
    SubsystemManager
)

add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:InferenceSubsystem>"
    "$<TARGET_FILE:SubsystemManager>"
    "$<TARGET_FILE_DIR:Application>"
)
