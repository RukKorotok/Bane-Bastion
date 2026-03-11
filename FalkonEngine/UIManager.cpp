#include "pch.h"

#include "UIManager.h"

namespace FalkonEngine {

//UIManager
//------------------------------------------------------------------------------------------------------------
void UIManager::UpdateHitbox(uint32_t entityID, const sf::FloatRect& bounds) {
    m_uiHitboxes[entityID] = bounds;
}

//------------------------------------------------------------------------------------------------------------
void UIManager::RemoveHitbox(uint32_t entityID) {
    m_uiHitboxes.erase(entityID);
}

//------------------------------------------------------------------------------------------------------------
void UIManager::Clear() { m_uiHitboxes.clear(); }

//------------------------------------------------------------------------------------------------------------
bool UIManager::IsPointerOverUI(const Vector2Df& mousePos) const {
    sf::Vector2f point(mousePos.x, mousePos.y);
    for (auto const& [id, rect] : m_uiHitboxes) {
      if (rect.contains(point)) {
        return true;
      }
    }
    return false;
  }

}  // namespace FalkonEngine