#pragma once

//

#include <string>
#include <cassert>

//

/// @brief Загрузчик модели.
class ModelLoader final
{
private:
    ///@brief Путь к файлу модели.
    char* modelFilePath;

    /// @brief Имя файла модели.
    char* modelFileName;

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

    /// @brief Устанавливает путь к файлу модели.
    void setPathToModelFile(char* path) { modelFilePath = path; }

    /// @brief Возвращает путь к файлу модели.
    /// @return Путь к файлу модели.
    const char* getPathToModelFile() const noexcept { return modelFilePath; }

    /// @brief Устанавливает имя файла модели.
    /// @param name Имя файла модели.
    void setModelFileName(char* name) { modelFileName = name; }

    /// @brief Возвращает имя файла модели.
    /// @return Имя файла модели.
    const char* getModelFileName() const noexcept { return modelFilePath; }

    /// @brief Возвращает модель.
    /// @details
    /// @return Модель.
    std::string getModel() const noexcept
    {
        assert(modelFilePath && modelFileName);

        if (!modelFilePath || !modelFileName)
        {
            return "";
        }

        return std::string(modelFilePath) + modelFileName;
    }
};
