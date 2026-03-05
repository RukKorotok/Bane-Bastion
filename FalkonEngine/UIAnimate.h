#pragma once

#include "UIAnimationComponent.h"
#include "UIImage.h"

namespace FalkonEngine {

/**
 * @brief High-level animated UI object.
 * Acts as a mediator between frame logic (UIAnimationComponent)
 * and visual rendering (UIImageComponent).
 */
class UIAnimate : public UIImage {
 public:
  /** @brief Inherit constructors from UIImage and UIPanel. */
  using UIImage::UIImage;

  /** @brief LIFECYCLE: Initializes components and subscribes to animation events. */
  void Awake() override;

  /**
   * @brief NOTIFICATION: Updates the sprite rect when notified of a frame change.
   * @param event The incoming animation frame event.
   */
  void OnNotify(const GameEvent& event) override;

  /**
   * @brief EXECUTION: Begins playback of the assigned sprite sheet.
   * @param path The file path to the texture atlas.
   */
  void Play(const std::string& path);

  /** @brief ACCESSOR: Returns the underlying animation component for frame setup. */
  UIAnimationComponent* GetAnimComponent() const { return p_animComponent; }

 private:
  /** @brief INTERNAL: Synchronizes the current frame rect to the image component. */
  void SyncFrame();

  /** @brief INTERNAL: Synchronizes sprite scale to match the current RectTransform size. */
  void LocalSyncImageScale();

 private:
  UIAnimationComponent* p_animComponent = nullptr;  ///< Logic component for frame timing.
};

}  // namespace FalkonEngine