#pragma once

//

/// @brief Конфигурация приложения.
struct ApplicationConfig final
{
    /// @brief Путь к файлу модели.
    char* pathToModelFile = {};
    /// @brief Путь к файлу оптимизированной модели.
    char* pathToOptimizedModelFile = {};
    /// @brief Имя файла модели.
    char* modelFileName = ".onnx";
};
