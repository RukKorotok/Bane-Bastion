#include "Block.h"
#include "Ball.h"
#include "Game.h"

//For next rev
namespace Bane_Bastion
{
	//Block
	//-----------------------------------------------------------------------------------------------------------
	Block::Block(int health, Vector2D size, Vector2D position) : GameObject(RESOURCES_PATH + "FrameClear.png", size, position)
	{
		SetColor(sf::Color::White);
		m_type = BlockType::Wall;
		UI::GetInstance().InitShape(m_blockShape, size.x, size.y, 0.9f, sf::Color::White, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::Visualize(sf::RenderWindow& window)
	{
		window.draw(m_blockShape);
		GameObject::Draw(window);
	}
	//ISerializable
	// -----------------------------------------------------------------------------------------------------------
	void Block::Serialize(std::ostream& out) const
	{
		out << m_type << " " << m_position.x << " " << m_position.y << " " << m_maxHealth << "\n";
	}
	//DestractedBlock
	//-----------------------------------------------------------------------------------------------------------
	DistractedBlock::DistractedBlock(int health, Vector2D size, Vector2D position) : Block(health, size, position)
	{
		assert(m_frameCrackTexture.loadFromFile(RESOURCES_PATH + "FrameCrack.png") );
		assert(m_frameBrokenTexture.loadFromFile(RESOURCES_PATH + "FrameBroken.png") );
		m_maxHealth = health;
		m_currentHealth = health;
		m_destroyer = nullptr;
		sf::Color color = sf::Color(100, 255 - health * 25, 255 - health * 25, 255);
		SetColor(sf::Color(color));
		m_type = BlockType::Normal;
		UI::GetInstance().InitShape(m_blockShape, size.x, size.y, 0.9f, sf::Color::White, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::OnHit(ICollidable& other)
	{
		m_currentHealth--;
		if (m_currentHealth <= 0)
		{
			StartTimer(0.20f);
			m_destroyer = &other;
		}
		else
		{
			UpdateTexture();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::UpdateBlock(float deltaTime)
	{
		UpdateTimer(deltaTime);
	}
	//-----------------------------------------------------------------------------------------------------------
	int DistractedBlock::GetMaxHealth()
	{
		return m_maxHealth;
	}
	//ISerializable
	// -----------------------------------------------------------------------------------------------------------
	void DistractedBlock::Serialize(std::ostream& out) const
	{
		Block::Serialize(out);
		out << m_currentHealth << "\n";
	}
	// -----------------------------------------------------------------------------------------------------------
	void DistractedBlock::Deserialize(std::istream& in)
	{
		if (in >> m_currentHealth)
		{
			UpdateTexture();
		}
		else
		{
			throw std::runtime_error("Ошибка данных при чтении Ball");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::FinalAction()
	{
		OnDestracted(*m_destroyer);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::EachTickAction(float deltaTime)
	{
		sf::Color spriteColor = m_sprite.getColor();
		sf::Color shapeColor = m_blockShape.getFillColor();

		if (spriteColor.a > 0)
		{
			spriteColor.a -= 10;
			shapeColor.a = 0;
		}
		m_sprite.setColor(spriteColor);
		m_blockShape.setFillColor(shapeColor);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::UpdateTexture()
	{
		float healthPersent = float(m_currentHealth) / float(m_maxHealth);

		if (healthPersent > 0.33f && healthPersent < 0.66f)
		{
			m_sprite.setTexture(m_frameCrackTexture);
		}
		else if (healthPersent < 0.33f)
		{
			m_sprite.setTexture(m_frameBrokenTexture);
		}
	}
	//BlockScoreUp
	//-----------------------------------------------------------------------------------------------------------
	BlockScoreUp::BlockScoreUp(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Magenta);
		m_type = BlockType::ScoreUp;
	}
	//-----------------------------------------------------------------------------------------------------------
	PoisonBlock::PoisonBlock(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Green);
		m_type = BlockType::Poison;
	}
	//DisorientBlock
	//-----------------------------------------------------------------------------------------------------------
	DisorientBlock::DisorientBlock(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Blue);
		m_type = BlockType::Disorient;
	}
}
