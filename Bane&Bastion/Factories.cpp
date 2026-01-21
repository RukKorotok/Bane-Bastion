#include "Factories.h"
#include "Block.h"
#include "Ball.h"
#include "Game.h"

namespace Bane_Bastion
{
	//Factory
	//-----------------------------------------------------------------------------------------------------------
	int Factory::GetCreatedItemsCount()
	{
		return m_createdBlocksCount;
	}

	void Factory::CleareCounter()
	{
		m_createdBlocksCount = 0;
	}

	void Factory::CounterAdd()
	{
		++m_createdBlocksCount;
	}

	//DistractedBlockFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Block> DistractedBlockFactory::CreateBlock(Vector2D position, Vector2D size, const int hitsCount)
	{
		BlockFactory::CounterAdd();
		return std::make_shared<DistractedBlock>(hitsCount, size, position);
	}

	//BlockScoreUpFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Block> BlockScoreUpFactory::CreateBlock(Vector2D position, Vector2D size, const int hitsCount)
	{
		BlockFactory::CounterAdd();
		return std::make_shared<BlockScoreUp>(hitsCount, size, position);
	}

	//PoisonBlockFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Block> PoisonBlockFactory::CreateBlock(Vector2D position, Vector2D size, const int hitsCount)
	{
		BlockFactory::CounterAdd();
		return std::make_shared<PoisonBlock>(hitsCount, size, position);
	}

	//DisorientBlockFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Block> DisorientBlockFactory::CreateBlock(Vector2D position, Vector2D size, const int hitsCount)
	{
		BlockFactory::CounterAdd();
		return std::make_shared<DisorientBlock>(hitsCount, size, position);
	}

	//WallBlockFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Block> WallBlockFactory::CreateBlock(Vector2D position, Vector2D size, const int hitsCount)
	{
		BlockFactory::CounterAdd();
		return std::make_shared<Block>(hitsCount, size, position);
	}

	//PrimaryBallFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Ball> PrimaryBallFactory::CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color)
	{
		return std::make_shared<MainBall>(size, position, ballDirection, color);
	}

	//PoisonBallFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Ball> PoisonBallFactory::CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color)
	{
		return std::make_shared<PoisonBall>(size, position, ballDirection, color);
	}

	//DisorientBallFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Ball> IncreasingBallFactory::CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color)
	{
		return std::make_shared<IncreasingBall>(size, position, ballDirection, color);
	}
	//GameStateFactory
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<GameState> GameStateFactory::CreateState(State state, int score)
	{
		if (state == State::GameInProgress)
		{
			return std::make_shared<GameStateInRuntime>(state, score);
		}
		return std::make_shared<GameMenuState>(state, score);
	}
}
