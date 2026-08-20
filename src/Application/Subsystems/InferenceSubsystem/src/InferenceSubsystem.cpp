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
        FLAG(isErrorAppeared)

        //

        ); // Статическое битовое поле.

    //

    if (!GET_FLAG_STATE(0, isStarted))
    {
        // Выполнение при первом запуске.

        // Проверка создания входных и выходных тензоров.

        if (inferenceContext.inputTensorNames.empty() || !inferenceContext.outputTensorNames.empty())
        {
            return 1;
        }

        try
        {
            // Запуск потока.

            inferenceThread = std::thread(&InferenceSubsystem::run, this);
        }
        catch(const std::exception& e)
        {
            return 1;
        }

        SET_FLAG(0, isStarted);
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
    DEBUG("Подсистема", subsystemHandle.name, "запущена");

    while (true)
    {
        if (!body()) { break; }
    }

    DEBUG("Подсистема", subsystemHandle.name, "");
}

/// @brief
/// @return
bool InferenceSubsystem::body()
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер битового поля в байт.

        //

        FLAG(isStarted)

        //

        ); // Статическое битовое поле.

    static Timer timerToTimeSinceStartThread(0u); // Таймер для отсчета периода времени с момента запуска потока.

    static std::chrono::steady_clock::duration start = {}; // Продолжительность для отсчета времени.

    // Запуск таймера для отсчета периода времени с момента запуска потока.

    if (timerManager.isStopped(timerToTimeSinceStartThread))
    {
        timerManager.start(timerToTimeSinceStartThread);
    }

    start = timerManager.getElapsedTime(timerToTimeSinceStartThread);

    //

    // Запуск конвейера.

    pipeline(timerToTimeSinceStartThread);

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

#define PROCESS(process) \
    start = timerManager.getElapsedTime(timer); \
    if (!process()) \
    { \
        SET_FLAG(0, isErrorAppeared); \
        break; \
    } \
    timerManager.getElapsedTime(timer) - start; \
    step++;

/// @brief Конвейер.
/// @brief Таймер для вычисления времен шагов вывода.
void InferenceSubsystem::pipeline(const Timer& timer)
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер битового поля в байт.

        //

        FLAG(isStarted)
        FLAG(isErrorAppeared)

        //

        ); // Статическое битовое поле.

    uint8_t step = {};

    std::chrono::steady_clock::duration start = {}; // Продолжительность для отсчета времени.

    //

    switch (step)
    {
    case 0u:
        PROCESS(prepareInputTensors)
    case 1u:
        PROCESS(inference)
    case 2U:
        PROCESS(prepareOutputTensors)

    default:
        break;
    }

    //

    if (GET_FLAG_STATE(0, isErrorAppeared))
    {
#ifndef NDEBUG
        SEPARATOR;

        DEBUG("Ошибка произошла на шаге:", step);

        //

        SEPARATOR;
#endif

        return;
    }
}

#undef PROCESS

namespace Inference
{
    namespace PrepareSettings
    {
        const uint8_t Reprepare = 1u; // Переподготовка.

        /*

        const uint8_t Option = 2u;

        */
    }// namespace PrepareSettings
} // namespace Inference

/// @brief Подготовка перед запуском вывода.
/// @warning При @ref Inference::PrepareSettings::Reprepare происходит стирание @ref Inference::inferenceContext, если подготовка была произведена.
/// @param options Опции. Дополнительно смотреть @ref Inference::PrepareSettings.
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

    if (options & PrepareSettings::Reprepare)
    {
        // Стирание битового поля.

        if (GET_FLAG_STATE(0, isCompleted))
        {
            reset();

            ERASE_BIT_FIELD(0);
        }
    }

    if (GET_FLAG_STATE(0, isCompleted)) { return; }

    //

    INFO("Запуск подготовки перед выводом");

    inferenceContext.threadingOptions = std::unique_ptr<Ort::ThreadingOptions>(new Ort::ThreadingOptions());

    // Установка количества потоков внутри операции.

    inferenceContext.threadingOptions->SetGlobalIntraOpNumThreads(4);

    // Установка количества потоков между операциями.

    inferenceContext.threadingOptions->SetGlobalInterOpNumThreads(1);

    // Создание окружения.

    inferenceContext.env = std::unique_ptr<Ort::Env>(new Ort::Env(*inferenceContext.threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference"));

    inferenceContext.sessionOptions = std::unique_ptr<Ort::SessionOptions>(new Ort::SessionOptions());

    inferenceContext.sessionOptions->DisablePerSessionThreads();

    // Проверка наличия оптимизированной модели.

    // Получение пути к оптимизированной модели.

    const char* pathToOptimizedModelFile = modelLoader->getPathToOptimizedModelFile();

    assert(pathToOptimizedModelFile);

    if (std::filesystem::exists(pathToOptimizedModelFile))
    {
        // Создание сессии.

        inferenceContext.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceContext.env, pathToOptimizedModelFile, *inferenceContext.sessionOptions));
    }
    else
    {
        WARNING("Файл оптимизированной модели по пути", modelLoader->getPathToOptimizedModelDirectory(), "отсутствует");

        // Установка уровня оптимизации модели.

        inferenceContext.sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

        DEBUG("Создание сессии...");

        // Установка пути к файлу оптимизированной модели.

        inferenceContext.sessionOptions->SetOptimizedModelFilePath("");

        // Создание сессии.

#ifndef NDEBUG
        // Получение пути к модели.

        const char* pathToModelFile = modelLoader->getPathToModelFile();

        assert(pathToModelFile);

        DEBUG("Путь к файлу модели:", pathToModelFile);

        inferenceContext.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceContext.env, pathToModelFile, *inferenceContext.sessionOptions));
#else
        inferenceContext.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceContext.env, modelLoader->getPathToModelFile(), *inferenceContext.sessionOptions));
#endif
    }

    // Создание входных и выходных тензоров.

    if (!createInputOutputTensors())
    {
        ERROR("Ошибка при создании входных и выходных тензоров");

        SET_FLAG(0, isErrorAppeared);
    }

    SET_FLAG(0, isCompleted);

    if (GET_FLAG_STATE(0, isErrorAppeared))
    {
        WARNING("Подготовка перед выводом была завершена c ошибками");

        reset();

        // Стирание битового поля.

        ERASE_BIT_FIELD(0);

        return;
    }
#if (1)
    else
    {
        WARNING("Подготовка перед выводом была завершена без ошибок");
    }
#endif
}

#ifndef NDEBUG
    /// @brief
    #define PRINT_TENSOR_SHAPE(tensorInfo) \
        do \
        { \
            LOG("Размерность:"); \
            LOG("["); \
            for (const auto& dim : *tensorInfo.shape) \
            { \
                dim != tensorInfo.shape->back() \
                    ? LOG(" ", dim, ",") \
                    : LOG(" ", dim); \
            } \
            LOG("]"); \
        } \
        while(0)
#endif

/// @brief Получение информации о модели.
/// @param handler Дескриптор вывода.
/// @return Информация о модели.
std::unique_ptr<Inference::ModelInfo> InferenceSubsystem::getModelInfo(const Inference::InferenceContext& handler)
{
    std::unique_ptr<Inference::ModelInfo> modelInfo = std::unique_ptr<Inference::ModelInfo>(new Inference::ModelInfo());

    Ort::AllocatorWithDefaultOptions allocator{{}};

    // Получение количества входов.

    modelInfo->inputCount = handler.session->GetInputCount();

    for (std::size_t i = 0; i < modelInfo->inputCount; i++)
    {
        // Получение информации о типе входов.

        const auto typeInfo = handler.session->GetInputTypeInfo(i);

        const auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

        // Получение имен входов.

        inferenceContext.modelInfo->inputTensorsInfo.at(i).name = handler.session->GetOutputNameAllocated(i, allocator).get();

        inferenceContext.inputTensorNames.push_back(inferenceContext.modelInfo->inputTensorsInfo.at(i).name.c_str());

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
            LOG(i++, ":");
            LOG("Имя", tensorInfo.name);

            PRINT_TENSOR_SHAPE(tensorInfo); // Смотреть выше.

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

        // Получение имен входов.

        inferenceContext.modelInfo->outputTensorsInfo.at(i).name = handler.session->GetOutputNameAllocated(i, allocator).get();

        inferenceContext.outputTensorNames.push_back(inferenceContext.modelInfo->outputTensorsInfo.at(i).name.c_str());

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
            LOG(i++, ":");
            LOG("Имя", tensorInfo.name);

            PRINT_TENSOR_SHAPE(tensorInfo); // Смотреть выше.

            LOG("Тип элементов:", tensorInfo.tensorElementDataType);
        }
    #endif
#endif

    return modelInfo;
}

#undef PRINT_TENSOR_SHAPE

/// @brief Создание входных и выходных тензоров.
/// @param
bool InferenceSubsystem::createInputOutputTensors()
{
    // Получение информации о модели.

    inferenceContext.modelInfo = getModelInfo(inferenceContext);

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // Создание входных тензоров.

    for (size_t i = 0; i < inferenceContext.modelInfo->inputCount; i++)
    {
        Inference::Tensor tensor = {};

        //

        assert(!inferenceContext.modelInfo->inputTensorsInfo.empty());

        assert(i <= inferenceContext.modelInfo->inputTensorsInfo.size());

        if (!inferenceContext.modelInfo->inputTensorsInfo.empty() && i <= inferenceContext.modelInfo->inputTensorsInfo.size())
        {
            tensor.metaData.shape = inferenceContext.modelInfo->inputTensorsInfo[i].shape;

            inferenceContext.inputTensorValues.push_back(
                Ort::Value::CreateTensor(
                    memoryInfo,

                    static_cast<void*>(tensor.rawData.data()),
                    tensor.rawData.size(),

                    tensor.metaData.shape->data(), // Указатель на размерность тензора.
                    tensor.metaData.shape->size(), //

                    inferenceContext.modelInfo->inputTensorsInfo[i].tensorElementDataType
                )
            );
        }
        else
        {
            return 1;
        }

        inferenceContext.inputTensors.push_back(std::move(tensor));
    }

    // Создание выходных тензоров.

    for (size_t i = 0; i < inferenceContext.modelInfo->outputCount; i++)
    {
        Inference::Tensor tensor = {};

        //

        assert(!inferenceContext.modelInfo->outputTensorsInfo.empty());

        assert(i <= inferenceContext.modelInfo->outputTensorsInfo.size());

        if (!inferenceContext.modelInfo->outputTensorsInfo.empty() && i <= inferenceContext.modelInfo->outputTensorsInfo.size())
        {
            tensor.metaData.shape = inferenceContext.modelInfo->outputTensorsInfo[i].shape;

            inferenceContext.outputTensorValues.push_back(
                Ort::Value::CreateTensor(
                    memoryInfo,

                    static_cast<void*>(tensor.rawData.data()),
                    tensor.rawData.size(),

                    tensor.metaData.shape->data(), // Указатель на размерность тензора.
                    tensor.metaData.shape->size(), //

                    inferenceContext.modelInfo->outputTensorsInfo[i].tensorElementDataType
                )
            );
        }
        else
        {
            return 1;
        }

        inferenceContext.outputTensors.push_back(std::move(tensor));
    }

    return 0;
}

/// @brief Подготовка входных тензоров.
bool InferenceSubsystem::prepareInputTensors()
{
    if (!inferenceContext.inputTensors.empty())
    {
        return false;
    }



    return true;
}

/// @brief
bool InferenceSubsystem::inference()
{
    return true;
}

/// @brief Подготовка выходных тензоров.
bool InferenceSubsystem::prepareOutputTensors()
{
    if (!inferenceContext.outputTensors.empty())
    {
        return false;
    }

    return true;
}

/// @brief
void InferenceSubsystem::reset()
{
    inferenceContext.reset();
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
