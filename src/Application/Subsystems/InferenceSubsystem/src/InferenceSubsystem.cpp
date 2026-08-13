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

    // Запуск таймера для отсчета периода времени с момента запуска потока.

    if (timerManager.isStopped(timerToTimeSinceStartThread))
    {
        timerManager.start(timerToTimeSinceStartThread);
    }

    if (0)
    {
        timerManager.getElapsedTime(timerToTimeSinceStartThread);

        // Остановка таймера для отсчета периода времени с момента запуска потока.

        timerManager.stop(timerToTimeSinceStartThread);

        return false;
    }

    return true;
}

/// @brief Подготовка перед запуском вывода.
void InferenceSubsystem::prepareBeforeStartInference()
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер битового поля в байт.

        //

        FLAG(isCompleted)

        //

        ); // Статическое битовое поле.

    //

    if (GET_BIT_FIELD(0).isCompleted) { return; }

    //

    INFO("Запуск подготовки перед выводом");

    //

    Ort::ThreadingOptions threadingOptions = {};

    // Установка количества потоков внутри операции.

    threadingOptions.SetGlobalIntraOpNumThreads(4);

    // Установка количества потоков между операциями.

    threadingOptions.SetGlobalInterOpNumThreads(1);

    // Создание окружения.

    inferenceHandler.env = std::unique_ptr<Ort::Env>(new Ort::Env(threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference"));

    Ort::SessionOptions sessionOptions = {};

    sessionOptions.DisablePerSessionThreads();

    // Проверка наличия оптимизированной модели.

    // Получение пути к оптимизированной модели.

    const char* pathToOptimizedModelFile = modelLoader->getPathToOptimizedModelFile();

    assert(pathToOptimizedModelFile);

    if (std::filesystem::exists(pathToOptimizedModelFile))
    {
        // Создание сессии.

        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, pathToOptimizedModelFile, sessionOptions));
    }
    else
    {
        WARNING("Файл оптимизированной модели по пути", modelLoader->getPathToOptimizedModelDirectory(), "отсутствует");

        // Установка уровня оптимизации модели.

        sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

        DEBUG("Создание сессии...");

        // Установка пути к файлу оптимизированной модели.

        sessionOptions.SetOptimizedModelFilePath("");

        // Создание сессии.

#ifndef NDEBUG
        // Получение пути к модели.

        const char* pathToModelFile = modelLoader->getPathToModelFile();

        assert(pathToModelFile);

        DEBUG("Путь к файлу модели:", pathToModelFile);

        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, pathToModelFile, sessionOptions));
#else
        inferenceHandler.session = std::unique_ptr<Ort::Session>(new Ort::Session(*inferenceHandler.env, modelLoader->getPathToModelFile(), sessionOptions));
#endif
    }

    // Получение информации о модели.

    modelInfo = getModelInfo(inferenceHandler);

    // Создание входных и выходных тензоров.

    if (createInputOutputTensors()) { return; }

    //

    INFO("Подготовка перед выводом завершена");

    //

    GET_BIT_FIELD(0).isCompleted = true;
}

/// @brief Создание входных и выходных тензоров.
/// @param inferenceHandler Дескриптор вывода.
/// @return Информация о выводе.
std::unique_ptr<Inference::ModelInfo> InferenceSubsystem::getModelInfo(const Inference::InferenceHandler& handler)
{
    std::unique_ptr<Inference::ModelInfo> modelInfo = std::unique_ptr<Inference::ModelInfo>(new Inference::ModelInfo());

    // Получение количества входов и выходов.

    modelInfo->inputCount = handler.session->GetInputCount();

    Inference::TensorInfo tensorInfo = {};

    for (std::size_t i = 0; i < modelInfo->inputCount; i++)
    {
        // Получение информации о типе входов.

        tensorInfo.typeInfo = handler.session->GetInputTypeInfo(i);

        tensorInfo.tensorTypeAndShapeInfo = tensorInfo.typeInfo.GetTensorTypeAndShapeInfo();

        // Получение типов данных элементов тензора.

        tensorInfo.tensorElementDataType = tensorInfo.tensorTypeAndShapeInfo.GetElementType();

        // Получение размерности тензора.

        tensorInfo.shape = tensorInfo.tensorTypeAndShapeInfo.GetShape();

        modelInfo->inputTensorsInfo.push_back(tensorInfo);
    }

#ifndef NDEBUG
    // Вывод информации о модели.

    #if (CUSTOM_CONFIGURATION_TURN_ON_OUTPUT_MODEL_INFO == 1)
        DEBUG("Входы:");
        DEBUG("Количество:", modelInfo->inputCount);

        DEBUG("Размерность:");

        for (const auto& item : modelInfo->inputTensorsInfo) { LOG(item.shape); }
    #endif
#endif

    modelInfo->outputCount = handler.session->GetInputCount();

    for (std::size_t i = 0; i < modelInfo->outputCount; i++)
    {
        // Получение информации о типе выходов.

        tensorInfo.typeInfo = handler.session->GetInputTypeInfo(i);

        tensorInfo.tensorTypeAndShapeInfo = tensorInfo.typeInfo.GetTensorTypeAndShapeInfo();

        // Получение размерности тензора.

        tensorInfo.shape = tensorInfo.tensorTypeAndShapeInfo.GetShape();

        modelInfo->outputTensorsInfo.push_back(tensorInfo);
    }

#ifndef NDEBUG
    // Вывод информации о модели.

    #if (CUSTOM_CONFIGURATION_TURN_ON_OUTPUT_MODEL_INFO == 1)
        DEBUG("Выходы:");
        DEBUG("Количество:", modelInfo->outputCount);

        DEBUG("Размерность:");

        for (const auto& item : modelInfo->outputTensorsInfo) { LOG(item.shape); }
    #endif
#endif

    return modelInfo;
}

/// @brief Создание входных и выходных тензоров.
/// @param
bool InferenceSubsystem::createInputOutputTensors()
{
    // Создание входных и выходных тензоров.

    try
    {
       inputTensor = std::unique_ptr<Inference::Tensor<Inference::TensorRawDataType>>(new Inference::Tensor<Inference::TensorRawDataType>());
    }
    catch(const std::exception& e)
    {
        return 1;
    }

    try
    {
       outputTensor = std::unique_ptr<Inference::Tensor<Inference::TensorRawDataType>>(new Inference::Tensor<Inference::TensorRawDataType>());
    }
    catch(const std::exception& e)
    {
        return 1;
    }

    // Описание входных и выходных тензоров.

    for (size_t i = 0; i < modelInfo->inputCount; i++)
    {

    }


    // Описание входных тензоров.

    inputTensor->value = Ort::Value::CreateTensor<Inference::TensorRawDataType>(
        inputTensor->metaData.info,

        inputTensor->rawData.data(),
        inputTensor->rawData.size(),

        inputTensor->metaData.shape.data(),
        inputTensor->metaData.shape.size()
    );

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
