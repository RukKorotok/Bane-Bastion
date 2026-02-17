#include "Scene.h"
#include "GameScene.h"
#include "Bastion.h"
#include "Bane.h"
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
		m_bastion.reset();
		m_bane.reset();
		m_npc.reset();

		FE_APP_TRACE("GameScene '" + GetName() + "' cleanup started.");

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
			m_bastion = std::make_shared<Bastion>(Vector2Df(centerX, centerY));
			m_bane = std::make_shared<Bane>(Vector2Df(centerX + 50.0f, centerY + 50.0f), m_bastion);
			m_bastion->Subscribe(m_bane.get());
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
	void GameScene::OnNotify(const GameEvent& event)
	{
		if (event.type == FalkonEngine::GameEventType::ObjectRemoved)
		{
			FalkonEngine::GameObject* removedObj = event.object;

			if (m_bastion && m_bastion->GetGameObject() == removedObj)
			{
				FE_APP_TRACE("GameScene: Player (Bastion) object removed. Resetting pointer.");
				m_bastion.reset();
				float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
				float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
				m_bastion = std::make_shared<Bastion>(Vector2Df(centerX, centerY));
				return;
			}

			if (m_bane && m_bane->GetGameObject() == removedObj)
			{
				FE_APP_TRACE("GameScene: Bane object removed. Resetting pointer.");
				m_bane.reset();
				return;
			}

			if (m_npc && m_npc->GetGameObject() == removedObj)
			{
				m_npc.reset();
				float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
				float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
				m_npc = std::make_shared<NPC>(Vector2Df(centerX + 100.0f, centerY + 100.0f), "NPC", "monster");
				return;
			}
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
		return m_bastion;
	}
}