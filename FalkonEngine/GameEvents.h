#pragma once

#include <any>
#include <map>
#include <string>
#include <typeindex>
#include "Vector.h"

namespace FalkonEngine {

class GameObject;

/**
 * @brief Enumeration of all possible event types in the engine bus.
 * Categorizes messages to allow Observers to quickly filter relevant data.
 */
enum class GameEventType {
  // Input
  InputChanged = 0,  ///< Keyboard/Joystick vector updates.
  MouseMoved,        ///< Screen/World space mouse coordinates.

  // Movement
  MovementRequested,  ///< Sent by AI/Input to trigger physics translation.
  MovementFinished,   ///< Destination reached (AI/Launch).
  PositionChanged,    ///< Broadcast after a successful transform update.

  // Physics
  HitVelocityRequest,  ///< Knockback forces applied to an entity.
  HitRotationRequest,  ///< Angular impulses (e.g., impact spin).

  // Game events
  StatChanged,      ///< Attribute updates (Health, Mana, etc.).
  ActionTriggered,  ///< Interaction or ability execution.
  ActionReleased,   ///< Interaction end.

  // Scene objects events
  ObjectRemoved,          ///< Entity destruction notification.
  ObjectSpawned,          ///< Base GameObject instantiation.
  ActorSpawned,           ///< Specialized Actor class instantiation.
  SceneComponentSpawned,  ///< Logic-only component instantiation.
  SubObjectSpawned,       ///< Child entity or projectile creation.

  // Scene Management
  SceneLoadRequest,     ///< Request for a complete scene swap.
  ScenePushRequest,     ///< Request to open a pause menu or overlay window.
  ScenePopRequest,      ///< Request to close the current overlay window.
  SceneRestartRequest,  ///< Request to restart the current level.

  // UI Actions
  PointerEnter,  ///< Mouse cursor entered the UI element bounds.
  PointerExit,   ///< Mouse cursor left the UI element bounds.
  PointerDown,   ///< Pointer button pressed over the UI element.
  PointerUp,     ///< Pointer button released.
  PointerMove    ///< Pointer moved within the UI element area.
};

/**
 * @brief A generic message packet used in the Observer pattern.
 * Provides a standardized way to pass data between engine systems.
 * * * Uses a Union to store mutually exclusive data types efficiently.
 * * Provides a type-safe template for retrieving the sender object.
 */
struct GameEvent {
  GameEventType type;                  ///< The category of this event.
  void* sender;                        ///< Raw pointer to the object that fired the event.
  size_t senderTypeId;                 ///< Hash of the sender's type for runtime validation.
  int entityID;                        ///< Unique ID of the GameObject involved.
  Vector2Df direction;                 ///< General purpose vector (Movement/Aiming).
  float angle;                         ///< General purpose float (Rotation/Spread).
  int actionID;                        ///< Identifier for specific stats or actions.
  std::map<std::string, float> input;  ///< Key-value map for dynamic attribute data.
  bool isConsumedByUI;                 ///< Flag indicating if the event was handled by the UI layer.

  /**
   * @brief Memory-efficient data storage for event-specific values.
   * Members within this union are mutually exclusive.
   */
  union {
    /**
     * @brief Rectangular data used for chunk updates or bounding box logic.
     */
    struct {
      float x, y, width, height;
    } area;

    float value;         ///< Generic scalar value (Damage, HP, Progress).
    GameObject* object;  ///< Direct pointer to a target GameObject.
  };

  /**
   * @brief Type-safe getter for the event sender.
   * Uses compile-time branching to handle both pointers and primitive types.
   * * @tparam T The expected type of the sender.
   * @return The casted sender data, or a default T() if type hash mismatch.
   */
  template <typename T>
  T GetSender() const {
    // Runtime Check: Verify if the requested type matches the stored type hash
    if (senderTypeId != typeid(T).hash_code()) {
      return T();  // Returns nullptr for pointers or 0 for numeric types
    }

    // Pointer handling
    if constexpr (std::is_pointer_v<T>) {
      return static_cast<T>(sender);
    } else {
      // Primitive handling (casting pointer value to integer types)
      return (T)(uintptr_t)sender;
    }
  }
};

}  // namespace FalkonEngine