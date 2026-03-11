#include "pch.h"

#include "DeferredActionRegistry.h"
#include "MusicPlayer.h"
#include "PauseMenu.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "SceneManager.h"
#include "UIAnimate.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UISlider.h"
#include "UIText.h"

using namespace FalkonEngine;

namespace BaneAndBastion {

// PauseMenu --- Implementation of pause screen UI logic ---
//-----------------------------------------------------------------------------------------------------------
PauseMenu::PauseMenu(const std::string& name) : Scene(name) {}

//-----------------------------------------------------------------------------------------------------------
PauseMenu::~PauseMenu() {
  // CLEANUP: Finalize scene resources
  FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");
}

//-----------------------------------------------------------------------------------------------------------
void PauseMenu::Start() {
  // COMMANDS: Bind UI interactions to the command registry
  const std::string resumeBinding = "escAction";
  const std::string mainMenuBinding = "MainMenuAction";
  const std::string exitBinding = "ExitGameAction";

  UICommandRegistry::Bind(resumeBinding, []() {
    FalkonEngine::DeferredActionRegistry::Instance().Push(
        []() { FalkonEngine::SceneManager::Instance().ApplyPopScene(); });
  });

  UICommandRegistry::Bind(mainMenuBinding, []() {
    FalkonEngine::DeferredActionRegistry::Instance().Push(
        []() { FalkonEngine::SceneManager::Instance().ApplyLoadScene("MainMenu"); });
  });

  UICommandRegistry::Bind(exitBinding, []() { FalkonEngine::RenderSystem::Instance()->GetMainWindow().close(); });

  // RESOURCES: Load required textures and font assets
  auto* resources = FalkonEngine::ResourceSystem::Instance();
  resources->LoadTexture("logo", "Resources/Textures/T_Logo.png");
  resources->LoadTexture("fireLogo", "Resources/Textures/AT_FireLogo.png");
  resources->LoadTexture("backGround", "Resources/Textures/T_MainMenuBackground.png");
  resources->LoadTexture("buttonBT", "Resources/Textures/T_BTBackground.png");

  resources->LoadFont("Font", "Resources/Fonts/EbonSigil-5yV1z.otf");

  FE_CORE_INFO("Starting GameScene: " + GetName());
  auto windowSize = FalkonEngine::RenderSystem::Instance()->GetMainWindow().getSize();

  try {
    auto world = GetWorld();

    // ENTITIES: Instantiate UI elements
    auto backgroundPanel = world->CreateGameObject<UIImage>("Background");
    auto fire = world->CreateGameObject<UIAnimate>("Fire");
    auto logo = world->CreateGameObject<UIImage>("Logo");
    auto resumeBT = world->CreateGameObject<UIButton>("StartGameBT");
    auto resumeTXT = world->CreateGameObject<UIText>("StartGameTXT");
    auto exitBT = world->CreateGameObject<UIButton>("ExitGameBT");
    auto exitTXT = world->CreateGameObject<UIText>("ExitGameTXT");
    auto mainMenuBT = world->CreateGameObject<UIButton>("mainMenuBT");
    auto mainMenuTXT = world->CreateGameObject<UIText>("mainMenuTXT");

    // HIERARCHY: Setup UI parent-child relationships
    backgroundPanel->AddChild(logo);
    backgroundPanel->AddChild(resumeBT);
    backgroundPanel->AddChild(exitBT);
    backgroundPanel->AddChild(mainMenuBT);

    logo->AddChild(fire);
    resumeBT->AddChild(resumeTXT);
    exitBT->AddChild(exitTXT);
    mainMenuBT->AddChild(mainMenuTXT);

    // LAYOUT: Configure background positioning and appearance
    auto transform = backgroundPanel->GetComponent<TransformComponent>();
    transform->SetLocalPosition({windowSize.x * 0.5f, windowSize.y * 0.5f});
    backgroundPanel->SetSize({float(windowSize.x), float(windowSize.y)});
    backgroundPanel->SetTexture("backGround");
    backgroundPanel->SetColor(sf::Color(0, 0, 0, 128));

    // LOGO: Configure header visuals
    logo->GetComponent<TransformComponent>()->SetLocalPosition({0.0f, -200.0f});
    logo->SetSize({700.0f, 300.0f});
    logo->SetTexture("logo");

    // ANIMATION: Setup sprite-sheet frames for the decorative fire effect
    fire->GetComponent<TransformComponent>()->SetLocalPosition({227.0f, -27.0f});
    fire->SetSize({64.0f, 64.0f});
    auto* animComp = fire->GetAnimComponent();

    animComp->AddFramesLine(0, 0, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 205, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 410, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 615, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 820, 205, 205, 5, 0.05f);
    animComp->SetSpeedMultiplier(0.75f);
    fire->Play("fireLogo");

    // BUTTONS: Initialize menu buttons with labels and actions
    resumeBT->GetComponent<TransformComponent>()->SetLocalPosition({0.0f, -50.0f});
    resumeBT->SetSize({350.0f, 75.0f});
    resumeBT->SetTexture("buttonBT");
    resumeBT->SetActionID(resumeBinding);

    resumeTXT->SetFont("Font");
    resumeTXT->SetFontSize(50);
    resumeTXT->SetColor(sf::Color(255, 0, 0, 200));
    resumeTXT->SetText("Resume game");

    mainMenuBT->GetComponent<TransformComponent>()->SetLocalPosition({0.0f, 50.0f});
    mainMenuBT->SetSize({350.0f, 75.0f});
    mainMenuBT->SetTexture("buttonBT");
    mainMenuBT->SetActionID(mainMenuBinding);

    mainMenuTXT->SetFont("Font");
    mainMenuTXT->SetFontSize(50);
    mainMenuTXT->SetColor(sf::Color(255, 0, 0, 200));
    mainMenuTXT->SetText("Main menu");

    exitBT->GetComponent<TransformComponent>()->SetLocalPosition({0.0f, 150.0f});
    exitBT->SetSize({350.0f, 75.0f});
    exitBT->SetTexture("buttonBT");
    exitBT->SetActionID(exitBinding);

    exitTXT->SetFont("Font");
    exitTXT->SetFontSize(50);
    exitTXT->SetColor(sf::Color(255, 0, 0, 200));
    exitTXT->SetText("Exit game");

    FE_CORE_INFO("UI HUD initialized successfully.");
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to initialize UI: " + std::string(e.what()));
  }

  // AUDIO: Start menu atmosphere
  MusicPlayer::Instance().Play("MainMenuMusic");
  MusicPlayer::Instance().SetVolume(20.f);
}

//-----------------------------------------------------------------------------------------------------------
void PauseMenu::Restart() {
  FE_CORE_INFO("Restarting GameScene...");
  Stop();
  Start();
}

//-----------------------------------------------------------------------------------------------------------
void PauseMenu::Stop() {
  FE_APP_TRACE("Stopping GameScene and clearing world.");
  if (GetWorld()) {
    MusicPlayer::Instance().Stop();
    // WORLD CLEAR: Remove all objects and clear command bindings
    GetWorld()->Clear();
    UICommandRegistry::Clear();
  }
}

//-----------------------------------------------------------------------------------------------------------
void PauseMenu::OnNotify(const GameEvent& event) {
  // Event-driven UI logic goes here
}

}  // namespace BaneAndBastion