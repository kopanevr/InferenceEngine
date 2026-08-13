cmake_minimum_required(VERSION 3.10)
project(InferenceEngine VERSION 0.1.0 LANGUAGES CXX)

#

option(CUSTOM_CONFIGURATION_TURN_ON_OUTPUT_MODEL_INFO "Включить вывод информации о модели при отладке." ON)

#

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application)
