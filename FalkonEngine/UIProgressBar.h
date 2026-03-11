#pragma once

#include "UIImage.h"

namespace FalkonEngine {

/**
 * @brief UI component representing a progress or status bar (HP, Mana, Exp).
 * Consists of a background image and a foreground "fill" image that scales
 * based on the current progress value. Supports event-driven updates.
 */
class UIProgressBar : public UIImage {
 public:
  /**
   * @brief Constructs a UIProgressBar.
   * @param name Unique name of the UI object.
   */
  UIProgressBar(std::string name);

  /**
   * @brief Default destructor.
   */
  virtual ~UIProgressBar() = default;

  /**
   * @brief Initializes the bar and creates the internal fill object.
   */
  virtual void Awake() override;

  /**
   * @brief Configures data binding to automatically update the bar based on game events.
   * @param type The type of event to listen for (e.g., StatChanged).
   * @param actionID Specific identifier for the stat (e.g., 1 for HP). Use -1 to ignore ID.
   * @param valKey Key in the event's data map for the current value.
   * @param maxKey Key in the event's data map for the maximum value.
   */
  void ConfigureBinding(GameEventType type, int actionID, std::string valKey, std::string maxKey);

  void SetPivot(Vector2Df pivot) override;
  /**
   * @brief Processes incoming notifications and updates progress if the event matches binding.
   * @param event The GameEvent containing stat updates.
   */
  virtual void OnNotify(const GameEvent& event) override;

  /**
   * @brief Manually sets the progress percentage.
   * @param percent Normalized value from 0.0 (empty) to 1.0 (full).
   */
  void SetProgress(float percent);

  /**
   * @brief Sets up textures and fill behavior for the progress bar.
   * @param bgPath Path to the background texture.
   * @param fillPath Path to the foreground/fill texture.
   * @param dir The direction in which the bar fills.
   */
  void SetProgressBarConfig(const std::string& bgPath, const std::string& fillPath, sf::Color color = sf::Color(0, 0, 0, 255), FillDirection dir = FillDirection::LeftToRight,
                            float topOffsetPers = 0.0f, float bottonOffsetPers = 0.0f);

 protected:
  /**
   * @brief Recalculates the size and texture rect of the fill object based on current progress.
   */
  void UpdateFillVisuals();

 private:
  UIImage* m_fillObject = nullptr;  ///< The inner visual object representing the "liquid" or fill level.
  float m_currentValue = 1.0f;      ///< Current fill percentage (0.0 to 1.0).

  // Event filtering parameters
  GameEventType m_targetEventType = GameEventType::StatChanged;
  int m_targetActionID = -1;
  std::string m_valueKey = "current";
  std::string m_maxKey = "max";
  FillDirection m_direction = FillDirection::LeftToRight;
  float m_topOffsetPers = 0.0f;
  float m_bottonOffsetPers = 0.0f;
};

}  // namespace FalkonEngine