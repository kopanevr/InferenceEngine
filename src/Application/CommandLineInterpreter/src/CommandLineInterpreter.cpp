#include "CommandLineInterpreter.hpp"

//

#include <getopt.h>

//

CommandLineInterpreter* CommandLineInterpreter::instance = {};

//

/// @brief Тело процесса.
/// @details
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
void CommandLineInterpreter::bodyProcess(int argc, char* argv[])
{
    int opt = {};

    const char* shortopts = ":hp:v";

    bool status = {};

    while ((opt = getopt(argc, argv, shortopts)) != -1)
    {
        switch (opt)
        {
        case 'h':
            break;
        case 'p':
            // Установка пути к директории модели.

            args.pathToModelDirectory = optarg;

            status = true;
            break;
        case 'v':
            break;
        case '?':
            break;
        case ':':
            break;
        default:
            break;
        }
    }

    isSuccessfullyParsed = status;
}
