/**
 * @file
 * @brief Подсистема вывода.
 */

#pragma once

//

#include <memory>
#include <thread>

//

#include "Subsystem.hpp"
#include "Logger.hpp"

//

#include "TimerManager.hpp"

//

class ModelLoader;

/// @brief Подсистема вывода.
class InferenceSubsystem final : public Subsystem
{
private:
    /// @brief Указатель на загрузчик модели.
    std::unique_ptr<ModelLoader> modelLoader;

    /// @brief Указатель на регистратор событий.
    std::shared_ptr<Logger> logger;

    /// @brief Поток вывода.
    std::thread inferenceThread;

    /// @brief Менеджер таймера.
    TimerManager timerManager;
private:
    /// @brief Конструктор.
    InferenceSubsystem();
    /// @brief Деструктор.
    ~InferenceSubsystem();
    InferenceSubsystem& operator=(const InferenceSubsystem&) = delete;
    InferenceSubsystem(const InferenceSubsystem&) = delete;

    /// @brief Инициализация подсистемы.
    void init() override;

    /// @brief Предварительная настройка перед запуском подсистемы.
    void setBeforeStartUp() override;

    /// @brief Предварительная настройка перед остановкой подсистемы.
    void setBeforeShutDown() override;

    /// @brief Тело процесса.
    int processBody() override;

    /// @brief
    void run();

    /// @brief
    bool body();

    /// @brief
    void prepareBeforeStartInference();
public:
    /// @brief Возвращает единственный экземпляр.
    /// @return Указатель на статический объект.
    static InferenceSubsystem* getInstance()
    {
        static InferenceSubsystem instance = {};

        return &instance;
    }

    /// @brief Устанавливает путь к директории модели.
    /// @param path Путь к директории модели.
    void setPathToModelDirectory(char* path);

    /// @brief Возвращает путь к файлу модели.
    /// @param name Имя файла модели.
    void setModelFileName(char* name);
};
