/**
 * @file
 * @brief Содержит описание интерпретатора команд.
 */

//

#include <cassert>

//

#include "CommandLineArgs.hpp"

//

/// @brief Интерпретатор команд.
class CommandLineInterpreter final
{
private:
    /// @brief Указатель на экземпляр.
    static CommandLineInterpreter* instance;

    /// @brief Аргументы.
    CommandLineArgs args;

    /// @brief Состояние обработки команд.
    bool isSuccessfullyParsed : 1;
private:
    /// @brief Конструктор.
    /// @param argc Количество аргументов.
    /// @param argv Указатель на список аргументов.
    CommandLineInterpreter(int argc, char* argv[])
    {
        process(argc, argv);
    }

    /// @brief Дружественный класс.
    friend class Application;

    /// @brief Тело процесса.
    /// @param argc Количество аргументов.
    /// @param argv Указатель на список аргументов.
    void bodyProcess(int argc, char* argv[]);

    /// @brief Процесс.
    void process(int argc, char* argv[])
    {
        bodyProcess(argc, argv);
    }
public:
    /// @brief Деструктор.
    ~CommandLineInterpreter() = default;

    /// @brief
    /// @return
    static CommandLineInterpreter& getInstance()
    {
        assert(instance);

        return *instance;
    }

    /// @brief Возвращает ссылку на аргументы.
    /// @return Ссылка на аргументы.
    const CommandLineArgs& getArgs() const noexcept { return args; }

    /// @brief
    /// @return
    bool isParsed() const noexcept { return isSuccessfullyParsed; }
};
