#pragma once

#include "Vector.h"
#include <any>
#include <typeindex>

namespace FalkonEngine
{
    class GameObject;

    /**
     * @brief Enumeration of all possible event types in the engine bus.
     * Categorizes messages to allow Observers to quickly filter relevant data.
     */
    enum class GameEventType
    {
        InputDirectionChanged = 0, ///< Keyboard/Joystick vector updates.
        MouseMoved,               ///< Screen/World space mouse coordinates.
        MovementRequested,        ///< Sent by AI/Input to trigger physics translation.
        HitVelocityRequest,       ///< Knockback forces applied to an entity.
        HitRotationRequest,       ///< Angular impulses (e.g., impact spin).
        PositionChanged,          ///< Broadcast after a successful transform update.
        StatChanged,              ///< Attribute updates (Health, Mana, etc.).
        ActionTriggered,          ///< Interaction or ability execution.
        ObjectRemoved,            ///< Entity destruction notification.
        ObjectSpawned,            ///< Base GameObject instantiation.
        ActorSpawned,             ///< Specialized Actor class instantiation.
        SceneComponentSpawned,    ///< Logic-only component instantiation.
        MovementFinished,         ///< Destination reached (AI/Launch).
        SubObjectSpawned          ///< Child entity or projectile creation.
    };

    /**
     * @brief A generic message packet used in the Observer pattern.
     * * Uses a Union to store mutually exclusive data types efficiently.
     * * Provides a type-safe template for retrieving the sender object.
     */
    struct GameEvent
    {
        GameEventType type;    ///< The category of this event.
        void* sender;          ///< Raw pointer to the object that fired the event.
        size_t senderTypeId;   ///< Hash of the sender's type for runtime validation.
        int entityID;          ///< Unique ID of the GameObject involved.
        Vector2Df direction;   ///< General purpose vector (Movement/Aiming).
        float angle;           ///< General purpose float (Rotation/Spread).
        int actionID;          ///< Identifier for specific stats or actions.

        /**
         * @brief Memory-efficient data storage for event-specific values.
         */
        union {
            struct { float x, y, width, height; } area; ///< Rectangular data (Chunk/Bounds).
            float value;                                ///< Generic scalar (Damage/HP).
            GameObject* object;                         ///< Direct pointer to a target object.
        };

        /**
         * @brief Type-safe getter for the event sender.
         * * Uses Compile-time branching (if constexpr) to handle pointers vs primitives.
         * @tparam T The expected type of the sender.
         * @return The casted sender pointer, or a default T() if types mismatch.
         */
        template<typename T>
        T GetSender() const
        {
            // Runtime Check: Verify if the requested type matches the stored type hash
            if (senderTypeId != typeid(T).hash_code())
            {
                return T(); // Returns nullptr for pointers or 0 for numeric types
            }

            // Pointer handling
            if constexpr (std::is_pointer_v<T>)
            {
                return static_cast<T>(sender);
            }
            else {
                // Primitive handling (casting pointer value to integer types)
                return (T)(uintptr_t)sender;
            }
        }
    };
}