#include "pch.h"

#include "Bane.h"
#include "Bastion.h"
#include "DeferredActionRegistry.h"
#include "ForestGenerator.h"
#include "ForestScene.h"
#include "GameUIBuilder.h"
#include "MazeGenerator.h"
#include "MazeNPCSpawner.h"
#include "NPC.h"
#include "PlayerController.h"
#include "Scene.h"
#include "SceneManager.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UISlider.h"
#include "UIText.h"

using namespace FalkonEngine;

namespace BaneAndBastion {

// ForestScene --- Implementation of procedural world scene logic ---
//-----------------------------------------------------------------------------------------------------------
ForestScene::ForestScene(const std::string& name) : Scene(name), m_gridManager(nullptr) {}

//-----------------------------------------------------------------------------------------------------------
ForestScene::~ForestScene() {
  // CLEANUP: Remove chunk-resident actors to prevent memory leaks
  for (auto it = m_chunkContent.begin(); it != m_chunkContent.end();) {
    ClearChunk(it->first);
    it = m_chunkContent.begin();
  }
  m_chunkContent.clear();

  // RELEASE: Reset smart pointers to ensure proper actor destruction
  m_bastion.reset();
  m_bane.reset();
  m_npcs.clear();

  FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");

  if (m_gridManager) {
    delete m_gridManager;
  }
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::Start() {
  std::string escBind = "Pause";
  auto* world = GetWorld();

  UICommandRegistry::Bind(escBind, []() {
    FalkonEngine::DeferredActionRegistry::Instance().Push(
        []() { FalkonEngine::SceneManager::Instance().ApplyPushScene("PauseMenu"); });
  });

  auto* resources = FalkonEngine::ResourceSystem::Instance();
  resources->LoadTexture("monster", "Resources/Textures/monster.png");
  resources->LoadTexture("tree", "Resources/Textures/T_Tree.png");
  resources->LoadTexture("wall", "Resources/Textures/T_Wall.png");
  resources->LoadTexture("bane", "Resources/Textures/T_Bane.png");
  resources->LoadTexture("bastion", "Resources/Textures/T_Bastion.png");

  resources->LoadTexture("monster_anim", "Resources/Textures/soldier_tilesheet.png");
  resources->LoadSound("monster_run", "Resources/Sounds/S_Silent-step.wav");

  if (!resources->LoadMusic("ForestMusic", "Resources/Music/S_Forest.wav")) {
    FE_CORE_WARN("Main: Initial music not loaded, but continuing...");
  }

  FE_CORE_INFO("Starting GameScene: " + GetName());

  // 1. PROCEDURAL SETUP: Initialize maze generator and NPC spawning logic
  auto generator = std::make_unique<ForestGenerator>();
  auto spawner = std::make_unique<MazeNPCSpawner>();
  m_gridManager = new GridManager(std::move(generator), std::move(spawner));

  // 2. SPAWN LOGIC: Calculate center coordinates for initial chunk population
  float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
  float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
  FalkonEngine::Vector2Df spawnPos(centerX, centerY);

  try {
    // 3. HERO INITIALIZATION: Spawn primary heroes and set up interaction/tethering
    m_bastion = std::make_shared<Bastion>(spawnPos);
    m_bastion->Init();
    m_bane = std::make_shared<Bane>(FalkonEngine::Vector2Df(centerX + 50.0f, centerY + 50.0f), m_bastion);
    m_bane->Init();

    // OBSERVER: Link Bane to Bastion movement events
    m_bastion->Subscribe(m_bane.get());

    auto* playerHUD = GameHUDBuilder::CreatePlayerHUD(world, m_bastion.get(), m_bane.get());

    FE_CORE_INFO("Heroes spawned successfully.");
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to initialize Heroes: " + std::string(e.what()));
    return;
  }

  // 4. GRID LOADING: Populate the immediate area around the player
  m_gridManager->UpdateVisibleArea(spawnPos, 0);

  // 5. AUDIO: Initialize atmosphere
  MusicPlayer::Instance().Play("ForestMusic");
  MusicPlayer::Instance().SetVolume(20.f);
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::Restart() {
  FE_CORE_INFO("Restarting GameScene...");
  Stop();
  Start();
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::Stop() {
  FE_APP_TRACE("Stopping GameScene and clearing world.");
  if (GetWorld()) {
    // WORLD CLEAR: Wipe all active GameObjects from physics and render systems
    GetWorld()->Clear();
  }
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::Resume() {
  std::string escBind = "Pause";
  UICommandRegistry::Bind(escBind, []() { FalkonEngine::SceneManager::Instance().ApplyPushScene("PauseMenu"); });
  MusicPlayer::Instance().Play("ForestMusic");
  MusicPlayer::Instance().SetVolume(20.f);
  m_isPaused = false;
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::OnNotify(const GameEvent& event) {
  switch (event.type) {
    case FalkonEngine::GameEventType::ObjectRemoved: {
      // CLEANUP: Handle entity removal upon destruction
      FalkonEngine::GameObject* removedObj = event.object;

      if (m_bastion && m_bastion->GetGameObject() == removedObj) {
        FE_APP_TRACE("GameScene: Bastion removed. Triggering respawn logic.");
        FalkonEngine::DeferredActionRegistry::Instance().Push(
            []() { FalkonEngine::SceneManager::Instance().ApplyLoadScene("MainMenu"); });
        return;
      }

      // TRACKING: Remove dead NPCs from local tracking list
      if (!m_npcs.empty()) {
        auto it = std::find_if(m_npcs.begin(), m_npcs.end(), [removedObj](const std::shared_ptr<NPC>& npc) {
          return npc->GetGameObject() == removedObj;
        });

        if (it != m_npcs.end()) {
          m_npcs.erase(it);
        }
      }
      break;
    }
    case FalkonEngine::GameEventType::ActionTriggered: {
      if (!m_isPaused) {
        std::string escBind = "Pause";
        auto it = event.input.find(escBind);
        if (it != event.input.end() && it->second == 1.0f) {
          UICommandRegistry::Execute(escBind);
          m_isPaused = true;
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::AddToChunk(FalkonEngine::Vector2Di chunkPos, FalkonEngine::Actor* obj) {
  if (!obj) return;
  m_chunkContent[chunkPos].push_back(obj);
}

//-----------------------------------------------------------------------------------------------------------
void ForestScene::ClearChunk(FalkonEngine::Vector2Di chunkPos) {
  // OPTIMIZATION: Unload procedural content for performance
  auto it = m_chunkContent.find(chunkPos);
  if (it != m_chunkContent.end()) {
    FE_APP_TRACE("Clearing chunk at [" + std::to_string(chunkPos.x) + ", " + std::to_string(chunkPos.y) + "]");

    for (auto* obj : it->second) {
      if (obj && obj->GetGameObject()) {
        GetWorld()->DestroyGameObject(obj->GetGameObject());
      }
      delete obj;
    }
    m_chunkContent.erase(it);
  }
}

//-----------------------------------------------------------------------------------------------------------
GridManager* ForestScene::GetGridManager() const { return m_gridManager; }

//-----------------------------------------------------------------------------------------------------------
std::shared_ptr<Player> ForestScene::GetPlayer() const {
  return m_bastion;  // Bastion serves as the primary AI target reference
}

}  // namespace BaneAndBastion