#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace FalkonEngine {

/**
 * @brief Data structure representing a single frame of animation.
 */
struct AnimationFrame {
  sf::IntRect rect;  ///< Source rectangle area within the texture atlas.
  float duration;    ///< Time in seconds to display this frame.
};

/**
 * @brief Container class for a sequence of animation frames.
 * Provides utility methods to easily slice textures into individual
 * frames for playback.
 */
class Animation {
 public:
  /**
   * @brief Default constructor.
   */
  Animation() = default;

  /**
   * @brief Manually adds a single frame to the animation sequence.
   * @param x Horizontal start position in pixels.
   * @param y Vertical start position in pixels.
   * @param w Frame width in pixels.
   * @param h Frame height in pixels.
   * @param duration Display time for this frame.
   */
  void AddFrame(int x, int y, int w, int h, float duration);

  /**
   * @brief Automatically slices and adds a horizontal line of frames from an atlas.
   * @param startX Initial X coordinate for the first frame.
   * @param startY Initial Y coordinate for the line.
   * @param width Width of each frame.
   * @param height Height of each frame.
   * @param count Total number of frames to add.
   * @param frameTime Duration for each added frame.
   */
  void AddFramesLine(int startX, int startY, int width, int height, int count, float frameTime);

  /**
   * @brief Retrieves a specific frame by its index.
   * @param index Position of the frame in the sequence.
   * @return Reference to the AnimationFrame data.
   */
  const AnimationFrame& GetFrame(size_t index) const;

  /**
   * @brief Gets the total number of frames in this animation.
   * @return The size of the frames collection.
   */
  size_t GetFrameCount() const;

 private:
  std::vector<AnimationFrame> m_frames;  ///< Ordered collection of animation frames.
};

}  // namespace FalkonEngine