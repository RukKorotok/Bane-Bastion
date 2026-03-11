#pragma once

#include <string>
#include "UIImage.h"
#include "UIText.h"
#include "Actor.h"

namespace BaneAndBastion {

class GameWorld;

/**
 * @brief Factory class for constructing standardized Player HUD.
 * Centralizes the creation of HP bars, ability icons, and status displays.
 */
class GameHUDBuilder {
 public:
  /**
   * @brief Builds and attaches the player interface to the specified world.
   * @param world The scene world where UI objects will be created.
   * @return UIImage* The root container of the HUD.
   */
  static FalkonEngine::UIImage* CreatePlayerHUD(FalkonEngine::GameWorld* world, FalkonEngine::Actor* bastion,
                                                FalkonEngine::Actor* bane);

};

}  // namespace FalkonEngine