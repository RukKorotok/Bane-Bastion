#include "pch.h"
#include "UICheckBox.h"
#include "UICommandRegistry.h"
#include "UIImageComponent.h"

namespace FalkonEngine {

// UICheckBox --- Interactive toggle component with dual visual layers ---
//------------------------------------------------------------------------------------------------------------
void UICheckBox::Awake() {
  // 1. Initialize background layer (via UISelectable -> UIImage hierarchy)
  UISelectable::Awake();

  // 2. LAYER COMPOSITION: Add the checkmark component on top of the background.
  // By adding it to the same GameObject, it shares the same transform but can be colored independently.
  m_markComponent = AddComponent<UIImageComponent>();
}

//------------------------------------------------------------------------------------------------------------
void UICheckBox::OnPointerDown(const GameEvent& e) {
  // TOGGLE LOGIC: Flip the state and refresh visuals
  m_isChecked = !m_isChecked;

  UpdateMark();

  // COMMAND DISPATCH: Execute the appropriate command based on the new state
  UICommandRegistry::Execute(m_isChecked ? p_actionID : m_actionIDOff);
}

//------------------------------------------------------------------------------------------------------------
void UICheckBox::SetCheckBoxConfig(const std::string& bgTxture, const std::string& markTxture,
                                   const std::string& actionOn, const std::string& actionOff, bool initialState) {
  p_actionID = actionOn;
  m_actionIDOff = actionOff;
  m_isChecked = initialState;

  // BACKGROUND: Set texture via UIImage base method
  SetTexture(bgTxture);

  // MARK: Set texture and sync its scale to match the checkbox dimensions
  if (m_markComponent) {
    m_markComponent->SetTexture(markTxture);

    // Ensure the mark fills the entire RectTransform area
    m_markComponent->SetScaleToSize(GetSize(), GetPivot());
  }

  UpdateMark();
}

//------------------------------------------------------------------------------------------------------------
void UICheckBox::UpdateMark() {
  if (m_markComponent) {
    sf::Color c = m_markComponent->GetColor();

    // VISIBILITY MANAGEMENT: Instead of disabling the component, we simply
    // toggle the Alpha channel (255 for opaque, 0 for transparent).
    c.a = m_isChecked ? 255 : 0;
    m_markComponent->SetColor(c);
  }
}

}  // namespace FalkonEngine