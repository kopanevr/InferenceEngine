cmake_minimum_required(VERSION 3.10)
project(InferenceWrapper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/InferenceSubsystem)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/SubsystemManager)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application)
