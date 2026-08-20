#include "FrameCaptureSubsystem.hpp"

//

#include "SubsystemId.hpp"

//

// Подсистемы.

#include "Logger.hpp"

//

#include "BitField.hpp"

//

/// @brief Конструктор.
FrameCaptureSubsystem::FrameCaptureSubsystem()
{
    init();
}

/// @brief Инициализация подсистемы.
void FrameCaptureSubsystem::init()
{
    subsystemHandle.id   = SubsystemId::FrameCaptureSubsystem;
    subsystemHandle.name = "FrameCapture";
}

/// @brief Предварительная настройка перед запуском подсистемы.
void FrameCaptureSubsystem::setBeforeStartUp()
{}

/// @brief Предварительная настройка перед остановкой подсистемы.
void FrameCaptureSubsystem::setBeforeShutDown()
{
    // Ожидание завершения потока.

    captureThread.join();
}

/// @brief Тело процесса.
int FrameCaptureSubsystem::processBody()
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

        try
        {
            // Запуск потока.

            captureThread = std::thread(&FrameCaptureSubsystem::run, this);
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
void FrameCaptureSubsystem::run()
{
    DEBUG("Подсистема", subsystemHandle.name, "запущена");

    while (true)
    {
        if (!body()) { break; }
    }

    DEBUG("Подсистема", subsystemHandle.name, "остановлена");
}

/// @brief
/// @return
bool FrameCaptureSubsystem::body()
{}