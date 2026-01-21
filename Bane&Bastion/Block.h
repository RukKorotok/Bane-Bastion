#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Objects.h"
#include "Math.h"
#include "Interfaces.h"
#include "SmothTimer.h"
#include "Observer.h"

namespace Bane_Bastion
{
	class Game;

	enum BlockType
	{
		Normal = 0,
		ScoreUp,
		Poison,
		Disorient,
		Wall
	};
	enum BlockDamage
	{
		Clear = 0,
		Crack,
		Broken
	};
	class Block : public GameObject, public ICollidable, public GameObservable, public ISerializable
	{
	public:

		Block(int health, Vector2D size, Vector2D position);

		void Visualize(sf::RenderWindow& window) override;
		virtual void UpdateBlock(float deltaTime) {}
		//ICollidable
		void OnHit(ICollidable& other) override {}
		//ISerializable
		void Serialize(std::ostream& out) const override;
		void Deserialize(std::istream& in) override {};

	protected:

		sf::RectangleShape m_blockShape;
		sf::Color m_color = sf::Color::White;
		BlockType m_type;
		int m_maxHealth = 1;
	};

	class DistractedBlock : public Block , public SmothTimer
	{
	public:

		DistractedBlock(int health, Vector2D size, Vector2D position);
		void UpdateBlock(float deltaTime) override;
		//ICollidable
		void OnHit(ICollidable& other) override;
		int GetMaxHealth();
		//ISerializable
		virtual void Serialize(std::ostream& out) const;
		virtual void Deserialize(std::istream& in);

	protected:

		void FinalAction() override;
		void EachTickAction(float deltaTime) override;
		void UpdateTexture();

		sf::Texture m_frameCrackTexture;
		sf::Texture m_frameBrokenTexture;
		sf::Color m_color = sf::Color::White;
		int m_currentHealth = 1;
		ICollidable* m_destroyer = nullptr;
	};


	class BlockScoreUp final : public DistractedBlock
	{
	public:

		BlockScoreUp(int health, Vector2D size, Vector2D position);

		//ISerializable
		virtual void Serialize(std::ostream& out) const { DistractedBlock::Serialize(out); }
		virtual void Deserialize(std::istream& in) { DistractedBlock::Deserialize(in); }

	private:
		//ICollidable
		void OnHit(ICollidable& other) override { DistractedBlock::OnHit(other); }
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

	class PoisonBlock final  : public DistractedBlock
	{
	public:

		PoisonBlock(int health, Vector2D size, Vector2D position);

		//ISerializable
		virtual void Serialize(std::ostream& out) const { DistractedBlock::Serialize(out); }
		virtual void Deserialize(std::istream& in) { DistractedBlock::Deserialize(in); }

	private:
		//ICollidable
		void OnHit(ICollidable& other) override { DistractedBlock::OnHit(other); }
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

	class DisorientBlock final : public DistractedBlock
	{
	public:

		DisorientBlock(int health, Vector2D size, Vector2D position);

		//ISerializable
		virtual void Serialize(std::ostream& out) const { DistractedBlock::Serialize(out); }
		virtual void Deserialize(std::istream& in) { DistractedBlock::Deserialize(in); }

	private:
		//ICollidable
		void OnHit(ICollidable& other) override { DistractedBlock::OnHit(other); }
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

}
