#include "Application.hpp"

//

#include <cstdlib>

//

#include "SubsystemManager.hpp"

//

/// @brief Конструктор.
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
Application::Application(int argc, char* argv[])
{
    commandLineInterpreter = std::unique_ptr<CommandLineInterpreter>(new CommandLineInterpreter(argc, argv));

    CommandLineInterpreter::instance = commandLineInterpreter.get();

    // Проверка состояния обработки аргументов.

    if (!commandLineInterpreter->isParsed())
    {
        isAllowedToExecute = false;

        return;
    }

    InferenceSubsystem* inferenceSubsystem = InferenceSubsystem::getInstance();

    // Установка имени файла модели.

    inferenceSubsystem->setModelFileName(applicationConfig.modelFileName);

    // Установка пути к файлу модели.

    // Проверка пути.

    if (commandLineInterpreter->getArgs().pathToModelFile)
    {
        // Установка пути к файлу модели, указанного в аргументах приложения при запуске.

        inferenceSubsystem->setPathToModelFile(commandLineInterpreter->getArgs().pathToModelFile);
    }
    else
    if (applicationConfig.pathToModelFile)
    {
        // Установка пути к файлу модели, указанного в конфигурации приложения.

        inferenceSubsystem->setPathToModelFile(applicationConfig.pathToModelFile);
    }
    else
    {
        isAllowedToExecute = false;

        return;
    }

    //

    subsystemManager = std::unique_ptr<SubsystemManager>(new SubsystemManager());

    SubsystemManager::instance = subsystemManager.get();

    // Инициализация.

    init();

    isAllowedToExecute = true;
}

/// @brief Деструктор.
Application::~Application()
{
    // Деинициализация.

    deinit();

    //

    SubsystemManager::instance = nullptr;
}

/// @brief Инициализация.
/// @details Производит запуск менеджера подсистем.
void Application::init()
{
    if (subsystemManager)
    {
        subsystemManager->startUp();
    }
}

/// @brief Выполнение.
/// @return Результат выполнения.
int Application::exec()
{
    if (!isAllowedToExecute) { return EXIT_FAILURE; }

    // Таймер для отсчета периода времени с момента запуска приложения.

    static Timer timerToTimeSinceStartApplication(0u);

    // Запуск таймера для отсчета периода времени с момента запуска приложения.

    if (timerManager.isStopped(timerToTimeSinceStartApplication))
    {
        timerManager.start(timerToTimeSinceStartApplication);
    }

    //

    int ret = subsystemManager->process();

    //

    timerManager.getElapsedTime(timerToTimeSinceStartApplication);

    // Остановка таймера для отсчета периода времени с момента запуска приложения.

    timerManager.stop(timerToTimeSinceStartApplication);

    if (ret == 1) { return EXIT_FAILURE; }

    return EXIT_SUCCESS;
}

/// @brief Деинициализация.
/// @details Производит остановку менеджера подсистем.
void Application::deinit()
{
    if (subsystemManager)
    {
        subsystemManager->shutDown();
    }
}
