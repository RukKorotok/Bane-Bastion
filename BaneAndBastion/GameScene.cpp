#include "pch.h"
#include "Scene.h"
#include "GameScene.h"
#include "Bastion.h"
#include "Bane.h"
#include "NPC.h"
#include "ForestGenerator.h"
#include "MazeNPCSpawner.h"
#include "MazeGenerator.h"

using namespace FalkonEngine;

namespace BaneAndBastion
{
    // GameScene Implementation
    // -----------------------------------------------------------------------------------------------------------
    GameScene::GameScene(const std::string& name) : Scene(name), m_gridManager(nullptr) {}

    // -----------------------------------------------------------------------------------------------------------
    GameScene::~GameScene()
    {
        // Explicitly release shared pointers to allow proper destruction of Actor components
        m_bastion.reset();
        m_bane.reset();
        m_npcs.clear();

        FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");

        // Manual cleanup of chunk-resident actors to prevent memory leaks
        for (auto& pair : m_chunkContent)
        {
            for (auto* obj : pair.second) delete obj;
        }
        m_chunkContent.clear();

        if (m_gridManager)
        {
            delete m_gridManager;
        }
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::Start()
    {
        FE_CORE_INFO("Starting GameScene: " + GetName());

        // 1. Procedural World Setup: Initialize the maze generator and NPC spawner
        auto generator = std::make_unique<MazeGenerator>();
        auto spawner = std::make_unique<MazeNPCSpawner>();
        m_gridManager = new GridManager(std::move(generator), std::move(spawner));

        // 2. Spawn Logic: Calculate the center point of the initial chunk
        float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
        float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
        FalkonEngine::Vector2Df spawnPos(centerX, centerY);

        try
        {
            // 3. Hero Initialization: Spawn Bastion and link Bane to him as a tethered partner
            m_bastion = std::make_shared<Bastion>(spawnPos);
            m_bane = std::make_shared<Bane>(FalkonEngine::Vector2Df(centerX + 50.0f, centerY + 50.0f), m_bastion);

            // Establish the Observer link so Bane can react to Bastion's movements
            m_bastion->Subscribe(m_bane.get());

            FE_CORE_INFO("Heroes spawned successfully.");
        }
        catch (const std::exception& e)
        {
            FE_CORE_ERROR("Failed to initialize Heroes: " + std::string(e.what()));
            return;
        }

        // 4. Initial Grid Load: Populate the area around the player
        m_gridManager->UpdateVisibleArea(spawnPos, 0);

        // 5. Audio: Initialize background atmosphere
        MusicPlayer::Instance().Play("NeverSurrender");
        MusicPlayer::Instance().SetVolume(20.f);
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::Restart()
    {
        FE_CORE_INFO("Restarting GameScene...");
        Stop();
        Start();
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::Stop()
    {
        FE_APP_TRACE("Stopping GameScene and clearing world.");
        if (GetWorld())
        {
            // Wipe all GameObjects from the physics/render world
            GetWorld()->Clear();
        }
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::OnNotify(const GameEvent& event)
    {
        switch (event.type)
        {
        case FalkonEngine::GameEventType::ObjectRemoved:
        {
            // Handle entity cleanup when GameObjects are destroyed (e.g., from death or scene transitions)
            FalkonEngine::GameObject* removedObj = event.object;

            if (m_bastion && m_bastion->GetGameObject() == removedObj)
            {
                FE_APP_TRACE("GameScene: Bastion removed. Triggering respawn logic.");
                m_bastion.reset();
                // Respawn at center (HACK for testing)
                float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                m_bastion = std::make_shared<Bastion>(Vector2Df(centerX, centerY));
                return;
            }

            if (m_bane && m_bane->GetGameObject() == removedObj)
            {
                m_bane.reset();
                return;
            }

            // Remove dead NPCs from the local tracking list
            if (!m_npcs.empty())
            {
                auto it = std::find_if(m_npcs.begin(), m_npcs.end(),
                    [removedObj](const std::shared_ptr<NPC>& npc) {
                        return npc->GetGameObject() == removedObj;
                    });

                if (it != m_npcs.end())
                {
                    m_npcs.erase(it);
                }
            }
            break;
        }
        }
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::AddToChunk(FalkonEngine::Vector2Di chunkPos, FalkonEngine::Actor* obj)
    {
        if (!obj) return;
        m_chunkContent[chunkPos].push_back(obj);
    }

    // -----------------------------------------------------------------------------------------------------------
    void GameScene::ClearChunk(FalkonEngine::Vector2Di chunkPos)
    {
        // Unload logic for procedural optimization
        auto it = m_chunkContent.find(chunkPos);
        if (it != m_chunkContent.end())
        {
            FE_APP_TRACE("Clearing chunk at [" + std::to_string(chunkPos.x) + ", " + std::to_string(chunkPos.y) + "]");

            for (auto* obj : it->second)
            {
                if (obj && obj->GetGameObject())
                {
                    GetWorld()->DestroyGameObject(obj->GetGameObject());
                }
                delete obj;
            }
            m_chunkContent.erase(it);
        }
    }

    // -----------------------------------------------------------------------------------------------------------
    GridManager* GameScene::GetGridManager() const
    {
        return m_gridManager;
    }

    // -----------------------------------------------------------------------------------------------------------
    std::shared_ptr<Player> GameScene::GetPlayer() const
    {
        return m_bastion; // Bastion is the primary player reference for AI targeting
    }
}