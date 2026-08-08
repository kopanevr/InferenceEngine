#pragma once

//

#include <string>
#include <cassert>

//

namespace ModelConfig
{
    /// @brief
    struct ModelPathConfig
    {
        /// @brief Путь к директории модели.
        char* modelDirectoryPath;

        /// @brief Имя файла модели.
        char* modelFileName;

        /// @brief Путь к файлу модели.
        std::string modelFilePath;
    };
} // namespace ModelConfig

//

/// @brief Загрузчик модели.
class ModelLoader final
{
private:
    /// @brief
    ModelConfig::ModelPathConfig modelPathConfig;

    /// @brief
    ModelConfig::ModelPathConfig optimizedModelPathConfig;

    /// @brief Указатель на экземпляр.
    static ModelLoader* instance;
private:
    /// @brief Конструктор.
    ModelLoader() = default;

    /// @brief
    friend class InferenceSubsystem;
public:
    /// @brief Деструктор.
    ~ModelLoader() = default;

    /// @brief
    /// @return
    ModelLoader& getInstance()
    {
        assert(instance);

        return *instance;
    }

    /// @brief Устанавливает путь к директории модели.
    /// @param name Путь к директории модели.
    void setPathToModelDirectory(char* path) noexcept { modelPathConfig.modelDirectoryPath = path; }

    /// @brief Возвращает путь к директории модели.
    /// @return Путь к директории модели.
    const char* getPathToModelDirectory() const noexcept { return modelPathConfig.modelDirectoryPath; }

    /// @brief Устанавливает имя файла модели.
    /// @param name Имя файла модели.
    void setModelFileName(char* name) noexcept { modelPathConfig.modelFileName = name; }

    /// @brief Возвращает имя файла модели.
    /// @return Имя файла модели.
    const char* getModelFileName() const noexcept { return modelPathConfig.modelFileName; }

    /// @brief Возвращает путь файлу модели.
    /// @details
    /// @return Путь к файлу модели.
    const char* getPathToModelFile() noexcept
    {
        assert(modelPathConfig.modelDirectoryPath && modelPathConfig.modelFileName);

        if (!modelPathConfig.modelDirectoryPath || !modelPathConfig.modelFileName)
        {
            return {};
        }

        (modelPathConfig.modelFilePath += modelPathConfig.modelDirectoryPath) += modelPathConfig.modelFileName;

        return modelPathConfig.modelFilePath.c_str();
    }

    /// @brief Возвращает путь файлу оптимизированной модели.
    /// @details
    /// @return Путь к файлу оптимизированной модели.
    const char* getPathToOptimizedModelFile() noexcept
    {
        assert(optimizedModelPathConfig.modelDirectoryPath && optimizedModelPathConfig.modelFileName);

        if (!optimizedModelPathConfig.modelDirectoryPath || !optimizedModelPathConfig.modelFileName)
        {
            return {};
        }

        (optimizedModelPathConfig.modelFilePath += optimizedModelPathConfig.modelDirectoryPath) += optimizedModelPathConfig.modelFileName;

        return optimizedModelPathConfig.modelFilePath.c_str();
    }
};
