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
    /// @brief
    using TensorRawDataType = double;

    /// @brief Информация о тензоре.
    struct TensorInfo
    {
        /// @brief Информация о типе.
        Ort::TypeInfo typeInfo;
        /// @brief Информация о типе и размерности тензора.
        Ort::ConstTensorTypeAndShapeInfo tensorTypeAndShapeInfo;
        /// @brief Тип данных элементов.
        ONNXTensorElementDataType tensorElementDataType;
        /// @brief Размерность тензора.
        std::vector<int64_t> shape;
    };

    /// @brief
    struct ModelInfo
    {
        /// @brief Количество входов.
        std::size_t inputCount;

        /// @brief Количество выходов.
        std::size_t outputCount;

        /// @brief Информация о входных тензорах.
        std::vector<TensorInfo> inputTensorsInfo;

        /// @brief Информация о выходных тензорах.
        std::vector<TensorInfo> outputTensorsInfo;
    };

    /// @brief Дескриптор вывода.
    struct InferenceHandler final
    {
        /// @brief Указатель на окружение.
        std::unique_ptr<Ort::Env> env;

        /// @brief Указатель на сессию.
        std::unique_ptr<Ort::Session> session;
    };

    /// @brief
    /// @tparam T Тип сырых данных тензора.
    template <typename T>
    struct Tensor final
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

    /// @brief Дескриптор вывода.
    Inference::InferenceHandler inferenceHandler;

    /// @brief Указатель на информацию о модели.
    std::unique_ptr<Inference::ModelInfo> modelInfo;

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

    /// @brief Получение информации о модели.
    /// @param handler Дескриптор вывода.
    [[nodiscard]] std::unique_ptr<Inference::ModelInfo> getModelInfo(const Inference::InferenceHandler& handler);

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
