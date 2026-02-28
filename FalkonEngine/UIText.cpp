#include "pch.h"

#include "UIText.h"

namespace FalkonEngine {

// UIText --- High-level UI element for rendering and managing text ---
//-----------------------------------------------------------------------------------------------------------
UIText::UIText(std::string name) : UIPanel(name) {
  // CONSTRUCTOR: Pass the object name to the base UIPanel class
}

//-----------------------------------------------------------------------------------------------------------
void UIText::Awake() {
  // INITIALIZATION: Setup base UI logic and RectTransform
  UIPanel::Awake();

  // COMPONENT BINDING: Add the low-level rendering component to the GameObject
  p_textComponent = AddComponent<UITextComponent>();
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetText(const std::string& text) {
  if (p_textComponent) {
    p_textComponent->SetString(text);
    RefreshTextLayout();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetFont(const std::string& path) {
  if (p_textComponent) {
    p_textComponent->SetFont(path);
    RefreshTextLayout();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetFontSize(unsigned int size) {
  if (p_textComponent) {
    p_textComponent->SetCharacterSize(size);
    RefreshTextLayout();
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetColor(sf::Color color) {
  if (p_textComponent) {
    p_textComponent->SetColor(color);
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetSize(Vector2Df size) {
  UIPanel::SetSize(size);
  RefreshTextLayout();
}

//-----------------------------------------------------------------------------------------------------------
void UIText::SetPivot(Vector2Df pivot) {
  UIPanel::SetPivot(pivot);
  RefreshTextLayout();
}

//-----------------------------------------------------------------------------------------------------------
void UIText::RefreshTextLayout() {
  if (p_textComponent) {
    // SYNCHRONIZATION: Update text origin based on current RectTransform bounds and pivot
    p_textComponent->SyncTransform(GetSize(), GetPivot());
  }
}

}  // namespace FalkonEngine