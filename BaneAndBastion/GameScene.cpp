#include "Scene.h"
#include "GameScene.h"
#include "Player.h"
#include "NPC.h"


using namespace FalkonEngine;

namespace BaneAndBastion
{
	//GameScene
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Start()
	{
		m_gridManager = new GridManager();
		m_gridManager->UpdateVisibleArea({ 0,0 }, 0);

		float centerX = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
		float centerY = (GameSettings::ChunkSize / 2.0f) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

		m_player = std::make_shared<Player>(FalkonEngine::Vector2Df(centerX, centerY), "Player", "knight");
		m_npc = std::make_shared<NPC>(FalkonEngine::Vector2Df(centerX + 50.0f, centerY + 50.0f), "NPC", "monster");
		MusicPlayer::Instance().Play("NeverSurrender");

		MusicPlayer::Instance().SetVolume(20.f);

	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Restart()
	{
		Stop();
		Start();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::Stop()
	{
		FalkonEngine::Scene::GetActive()->GetWorld()->Clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameScene::AddToChunk(FalkonEngine::Vector2Di chunkPos, BaneAndBastion::EnvironmentObject* obj)
	{
		m_chunkContent[chunkPos].push_back(obj);
	}
	//----------------------------------------------------------------------------------------------------------
	void GameScene::ClearChunk(FalkonEngine::Vector2Di chunkPos)
	{
		auto it = m_chunkContent.find(chunkPos);
		if (it != m_chunkContent.end()) {
			for (auto* obj : it->second) {
				this->GetWorld()->DestroyGameObject(obj->GetGameObject());
				delete obj;
			}
			m_chunkContent.erase(it);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	GridManager* GameScene::GetGridManager() const
	{
		return m_gridManager;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Player> GameScene::GetPlayer() const
	{
		return m_player;
	}
}