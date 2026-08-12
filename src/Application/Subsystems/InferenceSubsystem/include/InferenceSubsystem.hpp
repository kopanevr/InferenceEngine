/**
 * @file
 * @brief Подсистема вывода.
 */

#pragma once

//

#include <memory>
#include <thread>

//

#include "Subsystem.hpp"

//

#include "TimerManager.hpp"

//

#include "onnxruntime_cxx_api.h"

//

namespace Inference
{
    using TensorRawDataType = double;

    /// @brief
    /// @tparam T Тип сырых данных тензора.
    template <typename T>
    struct Tensor
    {
        struct MetaData
        {
            /// @brief
            Ort::MemoryInfo info{nullptr};

            /// @brief Размерность тензора.
            std::vector<int64_t> shape;
        }
        metaData;

        /// @brief Сырые данные тензора.
        std::vector<T> rawData;

        /// @brief  Тензор.
        Ort::Value value{nullptr};
    };
} // namespace Inference

class ModelLoader;

/// @brief Подсистема вывода.
class InferenceSubsystem final : public Subsystem
{
private:
    /// @brief Указатель на загрузчик модели.
    std::unique_ptr<ModelLoader> modelLoader;

    /// @brief Поток вывода.
    std::thread inferenceThread;

    /// @brief Менеджер таймера.
    TimerManager timerManager;

    /// @brief Указатель на входной буфер.
    std::unique_ptr<Inference::Tensor<Inference::TensorRawDataType>> inputTensor;
    /// @brief Указатель на выходной буфер.
    std::unique_ptr<Inference::Tensor<Inference::TensorRawDataType>> outputTensor;
private:
    /// @brief Конструктор.
    InferenceSubsystem();
    InferenceSubsystem& operator=(const InferenceSubsystem&) = delete;
    InferenceSubsystem(const InferenceSubsystem&) = delete;

    /// @brief Инициализация подсистемы.
    void init() override;

    /// @brief Предварительная настройка перед запуском подсистемы.
    void setBeforeStartUp() override;

    /// @brief Предварительная настройка перед остановкой подсистемы.
    void setBeforeShutDown() override;

    /// @brief Тело процесса.
    int processBody() override;

    /// @brief
    void run();

    /// @brief
    bool body();

    /// @brief Подготовка перед запуском вывода.
    void prepareBeforeStartInference();

    /// @brief Создание входных и выходных тензоров.
    /// @param
    bool createInputOutputTensors();
public:
    /// @brief Деструктор.
    ~InferenceSubsystem();

    /// @brief Возвращает единственный экземпляр.
    /// @return Указатель на статический объект.
    static InferenceSubsystem* getInstance()
    {
        static InferenceSubsystem instance = {};

        return &instance;
    }

    /// @brief Устанавливает путь к директории модели.
    /// @param path Путь к директории модели.
    void setPathToModelDirectory(char* path);

    /// @brief Возвращает путь к файлу модели.
    /// @param name Имя файла модели.
    void setModelFileName(char* name);
};
