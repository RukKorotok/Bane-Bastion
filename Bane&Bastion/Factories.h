#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace Bane_Bastion
{
	class Block;
	class Ball;
	class GameState;
	enum State : int;
	
	class Factory
	{
	public:
		virtual ~Factory() = default;
		int GetCreatedItemsCount();
		void CleareCounter();
		void CounterAdd();
	private:
		int m_createdBlocksCount = 0;
	};

	class BlockFactory : public Factory
	{
	public:
		virtual std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) = 0;
		virtual ~BlockFactory() = default;
	};

	class DistractedBlockFactory final : public BlockFactory
	{
	public:
		std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) override;
		~DistractedBlockFactory() override = default;
	};

	class BlockScoreUpFactory final : public BlockFactory
	{
	public:
		std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) override;
		~BlockScoreUpFactory() override = default;
	};

	class PoisonBlockFactory final : public BlockFactory
	{
	public:
		std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) override;
		~PoisonBlockFactory() override = default;
	};

	class DisorientBlockFactory final : public BlockFactory
	{
	public:
		std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) override;
		~DisorientBlockFactory() override = default;
	};

	class WallBlockFactory final : public BlockFactory
	{
	public:
		std::shared_ptr<Block> CreateBlock(Vector2D position, Vector2D size, const int hitsCount) override;
		~WallBlockFactory() override = default;
	};

	class BallFactory : public Factory
	{
	public:
		virtual std::shared_ptr<Ball> CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color) = 0;
		virtual ~BallFactory() = default;
	};

	class PrimaryBallFactory final : public BallFactory
	{
	public:
		std::shared_ptr<Ball> CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color) override;
		~PrimaryBallFactory() override = default;
	};

	class PoisonBallFactory final : public BallFactory
	{
	public:
		std::shared_ptr<Ball> CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color) override;
		~PoisonBallFactory() override = default;
	};

	class IncreasingBallFactory final : public BallFactory
	{
	public:
		std::shared_ptr<Ball> CreateBall(Vector2D position, Vector2D size, Vector2D ballDirection, sf::Color color) override;
		~IncreasingBallFactory() override = default;
	};

	class GameStateFactory : public Factory
	{
	public:
		virtual std::shared_ptr<GameState> CreateState(State state, int score);
		virtual ~GameStateFactory() = default;
	};
}