#pragma once

//

/// @brief Конфигурация приложения.
struct ApplicationConfig final
{
    /// @brief Путь к файлу модели.
    char* pathToModelFile = {};
    /// @brief Имя файла модели.
    char* modelFileName = ".onnx";
};
