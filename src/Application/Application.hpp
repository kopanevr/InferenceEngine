/**
 * @file
 * @brief Содержит описание приложения.
 */

#include <memory>

//

#include "ApplicationConfig.hpp"
#include "CommandLineInterpreter.hpp"

//

#include "TimerManager.hpp"

//

class SubsystemManager;

/// @brief Приложение.
class Application final
{
private:
    /// @brief Конфигурация приложения.
    ApplicationConfig applicationConfig;

    /// @brief Указатель на интерпретатор команд.
    std::unique_ptr<CommandLineInterpreter> commandLineInterpreter;

    /// @brief Указатель на менеджер подсистем.
    std::unique_ptr<SubsystemManager> subsystemManager;

    /// @brief Менеджер таймера.
    TimerManager timerManager;

    /// @brief
    bool isAllowedToExecute : 1;
private:
    /// @brief Конструктор.
    /// @param argc Количество аргументов.
    /// @param argv Указатель на список аргументов.
    Application(int argc, char* argv[]);
    ~Application();
public:
    /// @brief
    static Application* getInstance(int argc, char* argv[])
    {
        static Application instance(argc, argv);

        return &instance;
    }

    /// @brief Инициализация.
    /// @details Производит запуск менеджера подсистем.
    void init();
    /// @brief Деинициализация.
    /// @details Производит остановку менеджера подсистем.
    void deinit();

    /// @brief Выполнение.
    /// @return Результат выполнения.
    int exec();
};
