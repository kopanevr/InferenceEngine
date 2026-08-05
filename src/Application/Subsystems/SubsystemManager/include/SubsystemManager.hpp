/**
 * @file
 * @brief Содержит описание менеджера подсистем.
 * @details Для добавления подсистемы в список менеджера стоит воспользоваться @ref ADD_SUBSYSTEM.
 */

#include <array>

//

#include <cstddef>
#include <cassert>
#include <cstdlib>

//

#include "Subsystem.hpp"
#include "SubsystemId.hpp"

//

#include "InferenceSubsystem.hpp"

//

/// @brief Добавляет подсистему в список менеджера подсистем.
/// @param name Имя класса подсистемы, производного от @ref Subsystem.
#define ADD_SUBSYSTEM(name) \
    do \
    { \
        assert(i < subsystemCount); \
        subsystems[i++] = name::getInstance(); \
    } while(0)

//

/// @brief Менеджер подсистем.
class SubsystemManager final : public Subsystem
{
private:
    /// @brief Количество подсистем.
    static constexpr size_t subsystemCount = 1u;

    /// @brief Подсистемы.
    std::array<Subsystem*, subsystemCount> subsystems;

    /// @brief Указатель на объект-одиночку.
    static SubsystemManager* instance;
private:
    /// @brief Конструктор.
    SubsystemManager()
    {
        // Инициализация.

        init();

        size_t i = {};

        ADD_SUBSYSTEM(InferenceSubsystem);
    }

    /// @brief Дружественный класс.
    friend class Application;

    /// @brief Инициализация подсистемы.
    void init() override;
public:
    /// @brief Получает доступ к объекту.
    /// @return
    static SubsystemManager& getInstance()
    {
        assert(instance);

        return *instance;
    }

    /// @brief Настройка перед запуском подсистемы.
    void setBeforeStartUp() override
    {
        for (const auto& item : subsystems)
        {
            item->startUp();
        }
    }

    /// @brief Настройка перед остановкой подсистемы.
    void setBeforeShutDown() override
    {
        for (const auto& item : subsystems)
        {
            item->shutDown();
        }
    }

    /// @brief Тело основного цикла.
    int processBody() override
    {
        while (true)
        {
            for (const auto& item : subsystems)
            {
                if (item->process() == 1)
                {
                    return 1;
                }
            }
        }

        return 0;
    }

    /// @brief Возвращает количество подсистем.
    /// @return Количество подсистем.
    static constexpr size_t getSubsystemCount() noexcept { return subsystemCount; }

    /// @brief Возвращает подсистему по идентификатору.
    /// @param id Идентификатор.
    /// @return Указатель на подсистему.
    constexpr Subsystem* getSubsystemById(const SubsystemId id) const
    {
        for (const auto& item : subsystems)
        {
            if (item->getId() == id)
            {
                return item;
            }
        }

        return nullptr;
    }
};

#undef ADD_SUBSYSTEM
