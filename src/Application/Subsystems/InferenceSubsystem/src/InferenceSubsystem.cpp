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
    Ort::ThreadingOptions threadingOptions;

    // Установка количества потоков внутри операции.

    threadingOptions.SetGlobalIntraOpNumThreads(4);

    // Установка количества потоков между операциями.

    threadingOptions.SetGlobalInterOpNumThreads(1);

    // Создание окружения.

    Ort::Env env(threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference");
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

        ) = { .isStarted = {} }; // Статическое битовое поле.

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

/// @brief Устанавливает путь к файлу модели.
/// @param path Путь к файлу модели.
void InferenceSubsystem::setPathToModelFile(char* path)
{
    modelLoader->setPathToModelFile(path);
}

/// @brief
/// @param name Указатель на имя файла модели.
void InferenceSubsystem::setModelFileName(char* name)
{
    modelLoader->setModelFileName(name);
}
