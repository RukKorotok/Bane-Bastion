#include "pch.h"

#include "Bastion.h"
#include "DamageComponent.h"
#include "GameUIBuilder.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "Scene.h"
#include "StatDefinition.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UIText.h"

namespace BaneAndBastion {

// GameHUDBuilder --- UI factory for player HUD elements ---
//-----------------------------------------------------------------------------------------------------------
FalkonEngine::UIImage* GameHUDBuilder::CreatePlayerHUD(FalkonEngine::GameWorld* world, FalkonEngine::Actor* bastion,
                                                       FalkonEngine::Actor* bane) {
  auto* resources = FalkonEngine::ResourceSystem::Instance();

  // RESOURCES: Load textures required for HUD visualization
  resources->LoadTexture("Health", "Resources/Textures/T_AngelHealth.png");
  resources->LoadTexture("HealthBG", "Resources/Textures/T_AngelHealthBG.png");
  resources->LoadTexture("Mana", "Resources/Textures/T_DevilMana.png");
  resources->LoadTexture("ManaBG", "Resources/Textures/T_DevilManaBG.png");
  resources->LoadTexture("GameHudBackground", "Resources/Textures/T_GameHudBackground.png");
  resources->LoadTexture("GameHudBackgroundBG", "Resources/Textures/T_GameHudBackgroundBG.png");

  if (!resources->LoadMusic("ForestMusic", "Resources/Music/S_Forest.wav")) {
    FE_CORE_WARN("Main: Initial music not loaded, but continuing...");
  }

  resources->LoadFont("Font", "Resources/Fonts/EbonSigil-5yV1z.otf");
  auto windowSize = FalkonEngine::RenderSystem::Instance()->GetMainWindow().getSize();

  try {
    // ENTITIES: Instantiate primary UI components
    auto backgroundPanelBG = world->CreateGameObject<FalkonEngine::UIImage>("GameHudBackgroundBG");
    auto backgroundPanel = world->CreateGameObject<FalkonEngine::UIImage>("GameHudBackground");

    auto healthBG = world->CreateGameObject<FalkonEngine::UIImage>("HealthBG");
    auto health = world->CreateGameObject<FalkonEngine::UIProgressBar>("Health");

    auto manaBG = world->CreateGameObject<FalkonEngine::UIImage>("ManaBG");
    auto mana = world->CreateGameObject<FalkonEngine::UIProgressBar>("Mana");

    // HIERARCHY: Build the UI scene graph
    backgroundPanel->AddChild(backgroundPanelBG);
    backgroundPanel->AddChild(health);
    backgroundPanel->AddChild(mana);
    health->AddChild(healthBG);
    mana->AddChild(manaBG);

    // LAYOUT: Configure background positioning and size
    backgroundPanel->SetPivot({1.0f, 1.0f});
    backgroundPanel->GetComponent<FalkonEngine::TransformComponent>()->SetLocalPosition(
        {float(windowSize.x), float(windowSize.y)});
    backgroundPanel->SetSize({float(windowSize.x), 200.0f});
    backgroundPanel->SetTexture("GameHudBackground");

    backgroundPanelBG->SetPivot({1.0f, 1.0f});
    backgroundPanelBG->SetSize({float(windowSize.x), 200.0f});
    backgroundPanelBG->SetTexture("GameHudBackgroundBG");
    backgroundPanelBG->SetColor(sf::Color(0, 0, 0, 255));

    // BINDING: Initialize Health progress bar and observer data
    health->SetPivot({0.0f, 1.0f});
    health->GetComponent<FalkonEngine::TransformComponent>()->SetLocalPosition({135.0f - float(windowSize.x), 40.0f});
    health->SetSize({250.0f, 200.0f});
    health->SetProgressBarConfig("Health", "HealthBG", sf::Color(255, 0, 0, 255),
                                 FalkonEngine::FillDirection::BottomToTop, 0.05f, 0.30f);

    auto healthreg = StatRegistry::GetDefinition("Health");
    health->ConfigureBinding(FalkonEngine::GameEventType::StatChanged, 1, healthreg.currentKey, healthreg.maxKey);
    auto healthComponent = bastion->GetGameObject()->GetComponent<StatsComponent>();
    healthComponent->Subscribe(health);

    healthBG->SetPivot({0.0f, 1.0f});
    healthBG->SetSize({250.0f, 200.0f});
    healthBG->SetTexture("HealthBG");
    healthBG->SetColor(sf::Color(0, 0, 0, 255));

    // BINDING: Initialize Mana progress bar and observer data
    mana->SetPivot({1.0f, 1.0f});
    mana->GetComponent<FalkonEngine::TransformComponent>()->SetLocalPosition({-150.0f, 5.0f});
    mana->SetSize({260.0f, 180.0f});
    mana->SetProgressBarConfig("Mana", "ManaBG", sf::Color(0, 0, 255, 255), FalkonEngine::FillDirection::BottomToTop,
                               0.05f, 0.30f);

    auto manaReg = StatRegistry::GetDefinition("Hit");
    mana->ConfigureBinding(FalkonEngine::GameEventType::StatChanged, 1, manaReg.currentKey, manaReg.maxKey);
    auto manaComponent = bane->GetGameObject()->GetComponent<DamageComponent>();
    manaComponent->Subscribe(mana);

    manaBG->SetPivot({1.0f, 1.0f});
    manaBG->SetSize({260.0f, 180.0f});
    manaBG->SetTexture("ManaBG");
    manaBG->SetColor(sf::Color(0, 0, 0, 255));

    FE_CORE_INFO("UI HUD initialized successfully.");
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to initialize UI: " + std::string(e.what()));
  }

  // ROOT: Setup primary container for the HUD hierarchy
  auto* hudRoot = world->CreateGameObject<FalkonEngine::UIImage>("HUD_Root");
  hudRoot->SetSize({(float)windowSize.x, (float)windowSize.y});
  hudRoot->GetComponent<FalkonEngine::TransformComponent>()->SetLocalPosition(
      {windowSize.x * 0.5f, windowSize.y * 0.5f});

  FE_CORE_INFO("HUDFactory: Player HUD successfully constructed.");

  return hudRoot;
}

}  // namespace BaneAndBastion