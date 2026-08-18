#include "InferenceSubsystem.hpp"

//

#include <filesystem>
#include <vector>

//

#include "ModelLoader.hpp"
#include "SubsystemId.hpp"

//

// Подсистемы.

#include "Logger.hpp"

//

#include "BitField.hpp"

//

using namespace Inference;

//

/// @brief Конструктор.
InferenceSubsystem::InferenceSubsystem()
{
    modelLoader = std::unique_ptr<ModelLoader>(new ModelLoader());

    ModelLoader::instance = modelLoader.get();

    // Инициализация подсистемы.

    init();
}

/// @brief Деструктор.
InferenceSubsystem::~InferenceSubsystem()
{
    ModelLoader::instance = nullptr;
}

/// @brief Инициализация подсистемы.
void InferenceSubsystem::init()
{
    subsystemHandle.id   = SubsystemId::InferenceSubsystem;
    subsystemHandle.name = "Inference";
}

/// @brief Предварительная настройка перед запуском подсистемы.
void InferenceSubsystem::setBeforeStartUp()
{
    // Подготовка перед выводом.

    prepareBeforeStartInference();
}

/// @brief Предварительная настройка перед остановкой подсистемы.
void InferenceSubsystem::setBeforeShutDown()
{
    // Ожидание завершения потока.

    inferenceThread.join();
}

/// @brief Тело процесса.
int InferenceSubsystem::processBody()
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер битового поля в байт.

        //

        FLAG(isStarted)

        //

        ); // Статическое битовое поле.

    //

    if (!GET_BIT_FIELD(0).isStarted)
    {
        // Выполнение при первом запуске.

        try
        {
            // Запуск потока.

            inferenceThread = std::thread(&InferenceSubsystem::run, this);
        }
        catch(const std::exception& e)
        {
            return 1;
        }

        GET_BIT_FIELD(0).isStarted = true;

        DEBUG("Подсистема", subsystemHandle.name, "запущена");
    }
    else
    {
        // Выполнение при последующих запусках.

        //

        if (0)
        {
            // Стирание битового поля.

            ERASE_BIT_FIELD(0);

            return 1;
        }
    }

    return 0;
}

/// @brief
void InferenceSubsystem::run()
{
    while (true)
    {
        if (!body()) { break; }
    }
}

/// @brief
/// @return
bool InferenceSubsystem::body()
{
    // Таймер для отсчета периода времени с момента запуска потока.

    static Timer timerToTimeSinceStartThread(0u);

    static std::chrono::steady_clock::duration start = {}; // Продолжительность для отсчета времени вывода.

    // Запуск таймера для отсчета периода времени с момента запуска потока.

    if (timerManager.isStopped(timerToTimeSinceStartThread))
    {
        timerManager.start(timerToTimeSinceStartThread);
    }

    start = timerManager.getElapsedTime(timerToTimeSinceStartThread);

    //



    //

    profiler.inferencePeriod = timerManager.getElapsedTime(timerToTimeSinceStartThread) - start;

    if (0)
    {
        timerManager.getElapsedTime(timerToTimeSinceStartThread);

        // Остановка таймера для отсчета периода времени с момента запуска потока.

        timerManager.stop(timerToTimeSinceStartThread);

        return false;
    }

    return true;
}

namespace Inference
{
    namespace PrepareOptions
    {
        const uint8_t Reprepare = 1u; // Переподготовка.

        /*

        const uint8_t = 2u;

        */
    }// namespace PrepareOptions
} // namespace Inference

/// @brief Подготовка перед запуском вывода.
/// @warning
/// @param options Опции.
void InferenceSubsystem::prepareBeforeStartInference(uint8_t options)
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер битового поля в байт.

        //

        FLAG(isCompleted)
        FLAG(isErrorAppeared)

        //

        ); // Статическое битовое поле.

    //

    // Обработка флагов.

    // Переподготовка.

    if (options & PrepareOptions::Reprepare)
    {
        // Стирание битового поля.

        if (GET_BIT_FIELD(0).isCompleted)
        {
            ERASE_BIT_FIELD(0);

            //
        }
    }

    if (GET_BIT_FIELD(0).isCompleted) { return; }

    //

    INFO("Запуск подготовки перед выводом");

    inferenceHandler.threadingOptions = std::unique_ptr<Ort::ThreadingOptions>(new Ort::ThreadingOptions());

    // Установка количества потоков внутри операции.

    inferenceHandler.threadingOptions->SetGlobalIntraOpNumThreads(4);

    // Установка количества потоков между операциями.

    inferenceHandler.threadingOptions->SetGlobalInterOpNumThreads(1);

    // Создание окружения.

    inferenceHandler.env = std::unique_ptr<Ort::Env>(new Ort::Env(*inferenceHandler.threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference"));

    inferenceHandler.sessionOptions = std::unique_ptr<Ort::SessionOptions>(new Ort::SessionOptions());

    inferenceHandler.sessionOptions->DisablePerSessionThreads();

    // Проверка наличия оптимизированной модели.

    // Получение пути к оптимизированной модели.

    const char* pathToOptimizedModelFile = modelLoader->getPathToOptimizedModelFile();

    assert(pathToOptimizedModelFile);

    if (std::filesystem::exists(pathToOptimizedModelFile))
    {
        // Создание сессии.

        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, pathToOptimizedModelFile, *inferenceHandler.sessionOptions));
    }
    else
    {
        WARNING("Файл оптимизированной модели по пути", modelLoader->getPathToOptimizedModelDirectory(), "отсутствует");

        // Установка уровня оптимизации модели.

        inferenceHandler.sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

        DEBUG("Создание сессии...");

        // Установка пути к файлу оптимизированной модели.

        inferenceHandler.sessionOptions->SetOptimizedModelFilePath("");

        // Создание сессии.

#ifndef NDEBUG
        // Получение пути к модели.

        const char* pathToModelFile = modelLoader->getPathToModelFile();

        assert(pathToModelFile);

        DEBUG("Путь к файлу модели:", pathToModelFile);

        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, pathToModelFile, *inferenceHandler.sessionOptions));
#else
        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, modelLoader->getPathToModelFile(), *inferenceHandler.sessionOptions));
#endif
    }

    // Создание входных и выходных тензоров.

    if (!createInputOutputTensors())
    {
        ERROR("Ошибка при создании входных и выходных тензоров");

        GET_BIT_FIELD_NAME(0).isErrorAppeared = true;
    }

    GET_BIT_FIELD(0).isCompleted = true;

    if (GET_BIT_FIELD_NAME(0).isErrorAppeared)
    {
        WARNING("Подготовка перед выводом не была завершена");

        // Стирание битового поля.

        ERASE_BIT_FIELD(0);

        return ;
    }

    //

    INFO("Подготовка перед выводом была завершена");
}

/// @brief Получение информации о модели.
/// @param handler Дескриптор вывода.
/// @return Информация о модели.
std::unique_ptr<Inference::ModelInfo> InferenceSubsystem::getModelInfo(const Inference::InferenceHandler& handler)
{
    std::unique_ptr<Inference::ModelInfo> modelInfo = std::unique_ptr<Inference::ModelInfo>(new Inference::ModelInfo());

    // Получение количества входов.

    modelInfo->inputCount = handler.session->GetInputCount();

    for (std::size_t i = 0; i < modelInfo->inputCount; i++)
    {
        // Получение информации о типе входов.

        const auto typeInfo = handler.session->GetInputTypeInfo(i);

        const auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

        Inference::TensorInfo tensorInfo = {};

        // Получение типов данных элементов тензора.

        tensorInfo.tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();

        // Получение размерности тензора.

        try
        {
            tensorInfo.shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
        }
        catch(const std::exception& e)
        {
            //
        }

        modelInfo->inputTensorsInfo.push_back(std::move(tensorInfo));
    }

#ifndef NDEBUG
    // Вывод информации о входах.

    size_t i = 0; // Индекс тензора.

    #if (CUSTOM_CONFIGURATION_TURN_ON_OUTPUT_MODEL_INFO == 1)
        LOG("Входы:");
        LOG("Количество:", modelInfo->inputCount);

        for (const auto& tensorInfo : modelInfo->inputTensorsInfo)
        {
            LOG(i, ":");

            LOG("Размерность:");

            for (const auto& dim : *tensorInfo.shape)
            {
                LOG(dim);
            }

            LOG("Тип элементов:", tensorInfo.tensorElementDataType);
        }
    #endif
#endif

    // Получение количества выходов.

    modelInfo->outputCount = handler.session->GetOutputCount();

    for (std::size_t i = 0; i < modelInfo->outputCount; i++)
    {
        // Получение информации о типе входов.

        const auto typeInfo = handler.session->GetOutputTypeInfo(i);

        const auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

        Inference::TensorInfo tensorInfo = {};

        // Получение типов данных элементов тензора.

        tensorInfo.tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();

        // Получение размерности тензора.

        try
        {
            tensorInfo.shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
        }
        catch(const std::exception& e)
        {
            //
        }

        modelInfo->outputTensorsInfo.push_back(std::move(tensorInfo));
    }

#ifndef NDEBUG
    // Вывод информации о выходах.

    #if (CUSTOM_CONFIGURATION_TURN_ON_OUTPUT_MODEL_INFO == 1)
        LOG("Выходы:");
        LOG("Количество:", modelInfo->outputCount);

        i = 0;

        for (const auto& tensorInfo : modelInfo->outputTensorsInfo)
        {
            LOG(i, ":");

            LOG("Размерность:");

            for (const auto& dim : *tensorInfo.shape)
            {
                LOG(dim);
            }

            LOG("Тип элементов:", tensorInfo.tensorElementDataType);
        }
    #endif
#endif

    return modelInfo;
}

/// @brief Создание входных и выходных тензоров.
/// @param
bool InferenceSubsystem::createInputOutputTensors()
{
    // Получение информации о модели.

    inferenceHandler.modelInfo = getModelInfo(inferenceHandler);

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // Создание входных тензоров.

    for (size_t i = 0; i < inferenceHandler.modelInfo->inputCount; i++)
    {
        Inference::Tensor tensor = {};

        //

        tensor.metaData.shape = inferenceHandler.modelInfo->inputTensorsInfo.at(i).shape;

        tensor.value = Ort::Value::CreateTensor(
            memoryInfo,

            static_cast<void*>(tensor.rawData.data()),
            tensor.rawData.size(),

            tensor.metaData.shape->data(), // Указатель на размерность тензора.
            tensor.metaData.shape->size(), //

            inferenceHandler.modelInfo->inputTensorsInfo.at(i).tensorElementDataType
        );

        inferenceHandler.inputTensors.push_back(std::move(tensor));
    }

    // Создание выходных тензоров.

    for (size_t i = 0; i < inferenceHandler.modelInfo->outputCount; i++)
    {
        Inference::Tensor tensor = {};

        //

        tensor.metaData.shape = inferenceHandler.modelInfo->outputTensorsInfo.at(i).shape;

        tensor.value = Ort::Value::CreateTensor(
            memoryInfo,

            static_cast<void*>(tensor.rawData.data()),
            tensor.rawData.size(),

            tensor.metaData.shape->data(), // Указатель на размерность тензора.
            tensor.metaData.shape->size(), //

            inferenceHandler.modelInfo->inputTensorsInfo.at(i).tensorElementDataType
        );

        inferenceHandler.outputTensors.push_back(std::move(tensor));
    }

    return 0;
}

/// @brief Устанавливает путь к директории модели.
/// @param path Путь к директории модели.
void InferenceSubsystem::setPathToModelDirectory(char* path)
{
    modelLoader->setPathToModelDirectory(path);
}

/// @brief Устанавливает имя файла модели.
/// @param name Указатель на имя файла модели.
void InferenceSubsystem::setModelFileName(char* name)
{
    modelLoader->setModelFileName(name);
}
