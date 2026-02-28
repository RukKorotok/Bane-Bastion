#include "pch.h"

#include "UIImage.h"

namespace FalkonEngine {

// UIImage --- High-level wrapper for UI rendering logic ---
//-----------------------------------------------------------------------------------------------------------
void UIImage::Awake() {
  // INITIALIZATION: Execute base panel logic (handles RectTransform setup)
  UIPanel::Awake();

  // COMPONENT BINDING: Ensure every UIImage instance always possesses a rendering component.
  p_imageComponent = AddComponent<UIImageComponent>();
}

//-----------------------------------------------------------------------------------------------------------
void UIImage::SetTexture(const std::string& path) {
  if (p_imageComponent) {
    p_imageComponent->SetTexture(path);

    // RECALCULATION: Immediately adjust the sprite scale to fit the current RectTransform bounds.
    SyncImageScale();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIImage::SetColor(sf::Color color) {
  if (p_imageComponent) {
    p_imageComponent->SetColor(color);
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIImage::SetSize(Vector2Df size) {
  // DATA UPDATE: Update RectTransform dimensions via the base UIObject/UIPanel logic.
  UIPanel::SetSize(size);

  // VISUAL SYNC: Trigger an immediate scale update to match the new dimensions.
  SyncImageScale();
}

//-----------------------------------------------------------------------------------------------------------
void UIImage::SyncImageScale() {
  if (p_imageComponent) {
    // MEDIATION: Pass the logical size and pivot data down to the low-level rendering component.
    p_imageComponent->SetScaleToSize(GetSize(), GetPivot());
  }
}

}  // namespace FalkonEngine