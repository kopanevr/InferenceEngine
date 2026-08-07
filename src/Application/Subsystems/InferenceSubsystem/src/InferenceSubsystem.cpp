#include "InferenceSubsystem.hpp"

//

#include "ModelLoader.hpp"
#include "SubsystemId.hpp"

//

#include "BitField.hpp"

//

#include "onnxruntime_cxx_api.h"

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

/// @brief
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
        1, // Ожидаемый размер в байт.

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

/// @brief Подготавливает перед запуском вывода.
void InferenceSubsystem::prepareBeforeStartInference()
{
    STATIC_BIT_FIELD(
        0, // Идентификатор битового поля.
        1, // Ожидаемый размер в байт.

        //

        FLAG(isCompleted)

        //

        ); // Статическое битовое поле.

    //

    if (GET_BIT_FIELD(0).isCompleted) { return; }

    //

    Ort::ThreadingOptions threadingOptions = {};

    // Установка количества потоков внутри операции.

    threadingOptions.SetGlobalIntraOpNumThreads(4);

    // Установка количества потоков между операциями.

    threadingOptions.SetGlobalInterOpNumThreads(1);

    // Создание окружения.

    Ort::Env env(threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference");

    Ort::SessionOptions sessionOptions = {};

    sessionOptions.DisablePerSessionThreads();

    // Проверка наличия оптимизированной модели.

    if (0)
    {
        // Создание сессии.

#ifndef NDEBUG
        // Получение пути к оптимизированной модели.

        const char* pathToOptimizedModelFile = modelLoader->getPathToModelFile();

        assert(pathToOptimizedModelFile);

        Ort::Session session(env, pathToOptimizedModelFile, sessionOptions);
#else
        Ort::Session session(env, modelLoader->getPathToModelFile(), sessionOptions);
#endif
}
    else
    {
        // Установка уровня оптимизации модели.

        sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

        // Установка пути к файлу оптимизированной модели.

        sessionOptions.SetOptimizedModelFilePath("");

        // Создание сессии.

#ifndef NDEBUG
        // Получение пути к модели.

        const char* pathToModelFile = modelLoader->getPathToModelFile();

        assert(pathToModelFile);

        Ort::Session session(env, pathToModelFile, sessionOptions);
#else
        Ort::Session session(env, modelLoader->getPathToModelFile(), sessionOptions);
#endif
    }

    //

    GET_BIT_FIELD(0).isCompleted = true;
}

/// @brief Устанавливает путь к директории модели.
/// @param path Путь к директории модели.
void InferenceSubsystem::setPathToModelDirectory(char* path)
{
    modelLoader->setPathToModelDirectory(path);
}

/// @brief
/// @param name Указатель на имя файла модели.
void InferenceSubsystem::setModelFileName(char* name)
{
    modelLoader->setModelFileName(name);
}
