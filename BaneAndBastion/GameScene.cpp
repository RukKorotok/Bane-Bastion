#include "Scene.h"
#include "GameScene.h"
#include "Player.h"
#include "NPC.h"


using namespace FalkonEngine;

namespace BaneAndBastion
{
	//GameScene
	//-----------------------------------------------------------------------------------------------------------
	GameScene::GameScene(const std::string& name) : Scene(name), m_gridManager(nullptr) {}
	//-----------------------------------------------------------------------------------------------------------
	GameScene::~GameScene()
	{
		FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");
		if (m_gridManager) delete m_gridManager;

		for (auto& pair : m_chunkContent) 
		{
			for (auto* obj : pair.second) delete obj;
		}
		m_chunkContent.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Start()
	{
		FE_CORE_INFO("Starting GameScene: " + GetName());

		m_gridManager = new GridManager();
		m_gridManager->UpdateVisibleArea({ 0,0 }, 0);

		float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
		float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

		try 
		{
			m_player = std::make_shared<Player>(Vector2Df(centerX, centerY), "Player", "knight");
			m_npc = std::make_shared<NPC>(Vector2Df(centerX + 100.0f, centerY + 100.0f), "NPC", "monster");

			FE_CORE_INFO("Player and NPC successfully spawned.");
		}
		catch (const std::exception& e) 
		{
			FE_CORE_ERROR("Failed to initialize GameScene entities: " + std::string(e.what()));
		}

		MusicPlayer::Instance().Play("NeverSurrender");

		MusicPlayer::Instance().SetVolume(20.f);

	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Restart()
	{
		FE_CORE_INFO("Restarting GameScene...");
		Stop();
		Start();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Stop()
	{
		FE_APP_TRACE("Stopping GameScene and clearing world.");
		if(GetWorld())
		{
			GetWorld()->Clear();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::AddToChunk(FalkonEngine::Vector2Di chunkPos, BaneAndBastion::EnvironmentObject* obj)
	{
		if (!obj) 
		{
			return;
		}
		m_chunkContent[chunkPos].push_back(obj);
	}
	//----------------------------------------------------------------------------------------------------------
	void GameScene::ClearChunk(FalkonEngine::Vector2Di chunkPos)
	{
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
	//-----------------------------------------------------------------------------------------------------------
	GridManager* GameScene::GetGridManager() const
	{
		FE_CORE_ASSERT(m_gridManager != nullptr, "Attempted to get GridManager before it was initialized!");
		return m_gridManager;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Player> GameScene::GetPlayer() const
	{
		return m_player;
	}
}