#include "pch.h"

#include "UIAnimate.h"
#include "UIImageComponent.h"

namespace FalkonEngine {

// UIAnimate --- Mediator for frame-based UI animations ---
//-----------------------------------------------------------------------------------------------------------
void UIAnimate::Awake() {
  // BASE: Initialize rendering logic and inherited image components
  UIImage::Awake();

  // COMPONENT: Attach the logic controller for frame sequences and timing
  p_animComponent = AddComponent<UIAnimationComponent>();

  FE_APP_TRACE("UIAnimate: Awake for " + GetName());
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimate::OnNotify(const GameEvent& event) {
  // LISTENER: Handle frame updates dispatched by the animation component
  if (event.type == GameEventType::UpdatedAnimation) {
    SyncFrame();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimate::Play(const std::string& path) {
  if (p_imageComponent && p_animComponent) {
    // LOADING: Assign the texture atlas via string path
    p_imageComponent->SetTexture(path);

    // INITIALIZE: Render the first frame immediately
    SyncFrame();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimate::SyncFrame() {
  if (p_animComponent && p_imageComponent) {
    // DATA TRANSFER: Retrieve the current frame rect and apply it to the sprite
    p_imageComponent->SetTextureRect(p_animComponent->GetCurrentRect());

    // VISUAL SYNC: Adjust scale to ensure the texture fills the UI panel boundaries
    LocalSyncImageScale();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimate::LocalSyncImageScale() {
  if (p_imageComponent) {
    // SCALING: Recalculate sprite dimensions based on UI size and pivot
    p_imageComponent->SetScaleToSize(GetSize(), GetPivot());
  }
}

}  // namespace FalkonEngine