#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>
#include "Component.h"

namespace FalkonEngine {

/**
 * @brief Represents a single frame within a UI animation sequence.
 * Contains the texture region (rect) and its specific display time (duration).
 */
struct UIAnimationFrame {
  sf::IntRect rect;
  float duration;
};

/**
 * @brief Logic component responsible for managing frame sequences and timing.
 * Handles manual frame registration, automated line slicing, and playback speed control.
 */
class UIAnimationComponent : public Component {
 public:
  /** @brief Constructs the animation component and links it to a GameObject. */
  UIAnimationComponent(GameObject* gameObject);

  /**
   * @brief REGISTRATION: Push a manually defined frame into the sequence.
   * @param x, y The top-left coordinate of the frame in the atlas.
   * @param w, h The width and height of the frame.
   * @param duration Time in seconds this frame remains visible.
   */
  void AddFrame(int x, int y, int w, int h, float duration);

  /**
   * @brief GENERATION: Automatically slices a horizontal row from the texture atlas.
   * @param startX, startY Starting coordinates for the first frame.
   * @param width, height Dimensions of each frame in the row.
   * @param count Total number of frames to generate.
   * @param frameTime Default duration for each generated frame.
   */
  void AddFramesLine(int startX, int startY, int width, int height, int count, float frameTime);

  /**
   * @brief CONTROL: Sets the global playback speed multiplier.
   * @param multiplier 1.0f is normal, 2.0f is double speed, 0.5f is slow motion.
   */
  void SetSpeedMultiplier(float multiplier) { m_speedMultiplier = multiplier; }

  /** @brief CONFIGURATION: Sets the file path for the source texture sheet. */
  void SetTexturePath(const std::string& path) { m_texturePath = path; }

  /** @brief LIFECYCLE: Updates the animation timer and advances frames. */
  void Update(float deltaTime) override;

  /** @brief RENDERING: Unused as this is a logic-only component. */
  void Render() override {}

  /** @brief ACCESSOR: Returns the texture rect of the currently active frame. */
  const sf::IntRect& GetCurrentRect() const;

  /** @brief ACCESSOR: Returns the current texture atlas path. */
  const std::string& GetTexturePath() const { return m_texturePath; }

 private:
  std::vector<UIAnimationFrame> m_frames;  ///< Sequence of all registered frames.
  std::string m_texturePath;               ///< Path to the sprite sheet asset.

  float m_timer = 0.0f;                    ///< Internal clock for frame timing.
  float m_speedMultiplier = 1.0f;          ///< Playback rate modifier.
  size_t m_currentFrameIndex = 0;          ///< Index of the frame currently displayed.
  sf::IntRect m_emptyRect = {0, 0, 0, 0};  ///< Fallback rect for empty sequences.
};

}  // namespace FalkonEngine