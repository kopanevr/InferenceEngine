#pragma once

//

#include <string>
#include <cassert>

//

namespace ModelConfig
{
    /// @brief
    struct ModelPathConfig final
    {
        /// @brief Путь к директории модели.
        char* modelDirectoryPath;

        /// @brief Имя файла модели.
        char* modelFileName;

        /// @brief Путь к файлу модели.
        std::string modelFilePath;

        /// @brief Возвращает путь файлу модели.
        /// @details
        /// @return Путь к файлу модели.
        const char* getPathToModelFile() noexcept
        {
            assert(modelDirectoryPath && modelFileName);

            if (!modelDirectoryPath || !modelFileName)
            {
                return {};
            }

            (modelFilePath += modelDirectoryPath) += modelFileName;

            return modelFilePath.c_str();
        }
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

    /// @brief Устанавливает путь к директории оптимизированной модели.
    /// @param name Путь к директории оптимизированной модели.
    void setPathToOptimizedModelDirectory(char* path) noexcept { optimizedModelPathConfig.modelDirectoryPath = path; }

    /// @brief Возвращает путь к директории оптимизированной модели.
    /// @return Путь к директории оптимизированной модели.
    const char* getPathToOptimizedModelDirectory() const noexcept { return optimizedModelPathConfig.modelDirectoryPath; }

    /// @brief Устанавливает имя файла оптимизированной модели.
    /// @param name Имя файла оптимизированной модели.
    void setOptimizedModelFileName(char* name) noexcept { optimizedModelPathConfig.modelFileName = name; }

    /// @brief Возвращает имя файла оптимизированной модели.
    /// @return Имя файла оптимизированной модели.
    const char* getOptimizedModelFileName() const noexcept { return optimizedModelPathConfig.modelFileName; }

    /// @brief Возвращает путь файлу модели.
    /// @details
    /// @return Путь к файлу модели.
    const char* getPathToModelFile() noexcept { return modelPathConfig.getPathToModelFile(); }

    /// @brief Возвращает путь файлу оптимизированной модели.
    /// @details
    /// @return Путь к файлу оптимизированной модели.
    const char* getPathToOptimizedModelFile() noexcept { return optimizedModelPathConfig.getPathToModelFile();; }
};
