#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Vector.h"

namespace FalkonEngine {

class UIManager {
 public:
  static UIManager* Instance() {
    static UIManager instance;
    return &instance;
  }

  void UpdateHitbox(uint32_t entityID, const sf::FloatRect& bounds);
  void RemoveHitbox(uint32_t entityID);

  bool IsPointerOverUI(const Vector2Df& mousePos) const;

 private:
  UIManager() = default;
  std::map<uint32_t, sf::FloatRect> m_uiHitboxes;
};

}  // namespace FalkonEngine