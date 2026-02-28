#pragma once

#include "SubscriptionSystem.h"
#include "UIText.h"

namespace FalkonEngine {

/**
 * @brief A specialized UIText that automatically updates its content based on GameEvents.
 * Useful for HUD elements like HP numbers, mana, or score labels.
 */
class UIBoundText : public UIText {
 public:
  /**
   * @brief Constructs a UIBoundText object.
   * @param name Unique identifier for the GameObject.
   */
  UIBoundText(std::string name);

  /**
   * @brief Initializes the object and subscribes to the global event system.
   */
  void Awake() override;

  /**
   * @brief Configures which event and data key this text should listen to.
   * @param type The type of GameEvent to listen for.
   * @param key The key in the event's data map to extract the value from.
   * @param prefix Optional string to display before the value (e.g., "HP: ").
   * @param suffix Optional string to display after the value (e.g., " %").
   */
  void ConfigureBinding(GameEventType type, std::string key, std::string prefix = "", std::string suffix = "");

  /**
   * @brief Receives notifications from the EventSystem and updates the text string.
   * @param event The event data containing the new value.
   */
  void OnNotify(const GameEvent& event) override;

 private:
  GameEventType m_targetEventType;  ///< The event type we are interested in.
  std::string m_dataKey;            ///< The map key for the value we want to display.
  std::string m_prefix;             ///< Static text before the value.
  std::string m_suffix;             ///< Static text after the value.
};

}  // namespace FalkonEngine