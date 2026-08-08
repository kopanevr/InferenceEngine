add_library(
    InferenceSubsystem SHARED
    src/InferenceSubsystem.cpp
    src/ModelLoader.cpp
)

target_include_directories(
    InferenceSubsystem PUBLIC
    ${CMAKE_SOURCE_DIR}/third_party/onnxruntime-linux-x64-1.28.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
    ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/BitField/include
    ${CMAKE_SOURCE_DIR}/src/Application/Utilities/TimerManager/include
)

target_link_libraries(InferenceSubsystem PUBLIC Logger)

target_link_libraries(InferenceSubsystem PUBLIC ${CMAKE_SOURCE_DIR}/third_party/onnxruntime-linux-x64-1.28.0/lib/libonnxruntime.so)
