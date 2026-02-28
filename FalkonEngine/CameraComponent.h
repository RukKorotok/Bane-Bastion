#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "TransformComponent.h"

namespace FalkonEngine {

/**
 * @brief Component that defines the viewpoint of the scene.
 * Manages the SFML View (camera), allowing for zooming, following targets via
 * TransformComponent, and handling screen resolution scaling.
 */
class CameraComponent : public Component {
 public:
  /**
   * @brief Constructs a CameraComponent and links it to the owner's transform.
   * @param gameObject Pointer to the owner GameObject.
   */
  CameraComponent(GameObject* gameObject);

  /**
   * @brief Destructor. Cleans up the internal SFML View resource.
   */
  ~CameraComponent();

  /**
   * @brief Synchronizes the SFML View position with the GameObject's transform.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Applies the camera view to the associated RenderWindow.
   */
  void Render() override;

  // --- (Rendering) ---

  /** * @brief Assigns the target window where the camera view will be displayed.
   * @param newWindow Pointer to the SFML RenderWindow.
   */
  void SetWindow(sf::RenderWindow* newWindow);

  /** * @brief Adjusts the camera zoom level.
   * @param newZoom The zoom factor (e.g., > 1.0 to zoom out, < 1.0 to zoom in).
   */
  void ZoomBy(float newZoom);

  /** * @brief Sets the internal base resolution for the view.
   * @param width Width in pixels.
   * @param height Height in pixels.
   */
  void SetBaseResolution(int width, int height);

 private:
  TransformComponent* m_transform;  ///< Cached pointer to the owner's transform.
  sf::RenderWindow* m_window;       ///< Pointer to the active rendering window.
  sf::View* m_view;                 ///< Internal SFML View managing projection and viewport.
};

}  // namespace FalkonEngine