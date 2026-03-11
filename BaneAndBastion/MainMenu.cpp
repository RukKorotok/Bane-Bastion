#include "pch.h"

#include "MainMenu.h"
#include "SceneManager.h"
#include "UIAnimate.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UISlider.h"
#include "UIText.h"
#include  "DeferredActionRegistry.h"

using namespace FalkonEngine;

namespace BaneAndBastion {

// MainMenu Implementation --- Main entrance and UI setup ---
// -----------------------------------------------------------------------------------------------------------
MainMenu::MainMenu(const std::string& name) : Scene(name) {}

// -----------------------------------------------------------------------------------------------------------
MainMenu::~MainMenu() {
  // CLEANUP: Explicitly release shared pointers to allow proper destruction
  FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");
}

// -----------------------------------------------------------------------------------------------------------
void MainMenu::Start() {
  // COMMANDS: Bind UI actions to global command registry
  const std::string startGameBind = "StartGame";
  const std::string exitGameBind = "ExitGame";

  UICommandRegistry::Bind(startGameBind, []() {
    FalkonEngine::DeferredActionRegistry::Instance().Push(
        []() { FalkonEngine::SceneManager::Instance().ApplyLoadScene("Forest"); });
  });

  UICommandRegistry::Bind(exitGameBind, []() { FalkonEngine::RenderSystem::Instance()->GetMainWindow().close(); });
  // RESOURCES: Load textures, fonts, and music into the global resource system
  auto* resources = FalkonEngine::ResourceSystem::Instance();
  resources->LoadTexture("logo", "Resources/Textures/T_Logo.png");
  resources->LoadTexture("fireLogo", "Resources/Textures/AT_FireLogo.png");
  resources->LoadTexture("backGround", "Resources/Textures/T_MainMenuBackground.png");
  resources->LoadTexture("buttonBT", "Resources/Textures/T_BTBackground.png");

  if (!resources->LoadMusic("MainMenuMusic", "Resources/Music/S_MainMenu.wav")) {
    FE_CORE_WARN("Main: Initial music not loaded, but continuing...");
  }

  resources->LoadFont("Font", "Resources/Fonts/EbonSigil-5yV1z.otf");

  FE_CORE_INFO("Starting GameScene: " + GetName());
  auto windowSize = FalkonEngine::RenderSystem::Instance()->GetMainWindow().getSize();

  try {
    auto world = GetWorld();

    // ENTITIES: Create primary UI objects
    auto backgroundPanel = world->CreateGameObject<UIImage>("Background");
    auto fire = world->CreateGameObject<UIAnimate>("Fire");
    auto logo = world->CreateGameObject<UIImage>("Logo");
    auto startGameBT = world->CreateGameObject<UIButton>("StartGameBT");
    auto startGameTXT = world->CreateGameObject<UIText>("StartGameTXT");
    auto exitGameBT = world->CreateGameObject<UIButton>("ExitGameBT");
    auto exitGameTXT = world->CreateGameObject<UIText>("ExitGameTXT");

    // HIERARCHY: Setup parent-child relationships for UI tree
    backgroundPanel->AddChild(logo);
    backgroundPanel->AddChild(startGameBT);
    backgroundPanel->AddChild(exitGameBT);
    logo->AddChild(fire);
    startGameBT->AddChild(startGameTXT);
    exitGameBT->AddChild(exitGameTXT);

    // TRANSFORM & VISUALS: Configure background
    auto transform = backgroundPanel->GetComponent<TransformComponent>();
    transform->SetLocalPosition({windowSize.x * 0.5f, windowSize.y * 0.5f});
    backgroundPanel->SetSize({float(windowSize.x), float(windowSize.y)});
    backgroundPanel->SetTexture("backGround");

    // LOGO: Configure header visuals
    logo->GetComponent<TransformComponent>()->SetLocalPosition({-250.0f, -100.0f});
    logo->SetSize({700.0f, 300.0f});
    logo->SetTexture("logo");

    // ANIMATION: Configure fire effect sprite-sheet
    fire->GetComponent<TransformComponent>()->SetLocalPosition({227.0f, -27.0f});
    fire->SetSize({64.0f, 64.0f});
    auto* animComp = fire->GetAnimComponent();

    // SLICING: Manually defining frame lines from the fire atlas
    animComp->AddFramesLine(0, 0, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 205, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 410, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 615, 205, 205, 5, 0.05f);
    animComp->AddFramesLine(0, 820, 205, 205, 5, 0.05f);
    animComp->SetSpeedMultiplier(0.75f);
    fire->Play("fireLogo");

    // BUTTONS: Setup 'Start Game' interaction
    startGameBT->GetComponent<TransformComponent>()->SetLocalPosition({400.0f, -150.0f});
    startGameBT->SetSize({350.0f, 75.0f});
    startGameBT->SetTexture("buttonBT");
    startGameBT->SetActionID(startGameBind);

    startGameTXT->SetFont("Font");
    startGameTXT->SetFontSize(50);
    startGameTXT->SetColor(sf::Color(255, 0, 0, 200));
    startGameTXT->SetText("Start game");

    // BUTTONS: Setup 'Exit Game' interaction
    exitGameBT->GetComponent<TransformComponent>()->SetLocalPosition({400.0f, -50.0f});
    exitGameBT->SetSize({350.0f, 75.0f});
    exitGameBT->SetTexture("buttonBT");
    exitGameBT->SetActionID(exitGameBind);

    exitGameTXT->SetFont("Font");
    exitGameTXT->SetFontSize(50);
    exitGameTXT->SetColor(sf::Color(255, 0, 0, 200));
    exitGameTXT->SetText("Exit game");

    FE_CORE_INFO("UI HUD initialized successfully.");
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to initialize UI: " + std::string(e.what()));
  }

  // AUDIO: Initialize background atmosphere
  MusicPlayer::Instance().Play("MainMenuMusic");
  MusicPlayer::Instance().SetVolume(20.f);
}

// -----------------------------------------------------------------------------------------------------------
void MainMenu::Restart() {
  FE_CORE_INFO("Restarting GameScene...");
  Stop();
  Start();
}

// -----------------------------------------------------------------------------------------------------------
void MainMenu::Stop() {
  FE_APP_TRACE("Stopping GameScene and clearing world.");
  if (GetWorld()) {
    MusicPlayer::Instance().Stop();
    // WIPE: Clear all GameObjects from the render/physics world
    GetWorld()->Clear();
    UICommandRegistry::Clear();
  }
}

// -----------------------------------------------------------------------------------------------------------
void MainMenu::OnNotify(const GameEvent& event) {
  // EVENT HANDLING: Scene-specific notification logic
}

}  // namespace BaneAndBastion