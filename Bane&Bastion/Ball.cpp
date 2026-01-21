#include "Game.h"
#include "Ball.h"
namespace Bane_Bastion
{
	//Ball
	//------------------------------------------------------------------------------------------------------------
	Ball::Ball(Vector2D size, Vector2D position, Vector2D vectorSpeed) : GameObject(RESOURCES_PATH + "Ball.png", size, position)
	{
		m_scalarSpeed = INITIAL_BALL_SPEED;
		m_vectorSpeed = vectorSpeed;
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBall(float deltaTime, sf::RenderWindow& window)
	{
		CheckCollisions();
		UpdateBallPosition(deltaTime);
		Draw(window);
	}
	//protected
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBallPosition(float deltaTime)
	{
		m_position = { m_position.x + m_scalarSpeed * m_vectorSpeed.x * deltaTime, m_position.y + m_scalarSpeed * m_vectorSpeed.y * deltaTime };
		m_sprite.setPosition(m_position.x, m_position.y);
	}
	//MainBall
	//------------------------------------------------------------------------------------------------------------
	MainBall::MainBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color) : Ball(size, position, vectorSpeed)
	{
		m_normalSpeed = m_scalarSpeed;
		SetColor(color);
	}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::SetPosition(Vector2D position)
	{
		m_position = position;
	}
	//----------------------------------------------------------------------------------------------------------
	void MainBall::SetVectorSpeed(Vector2D vectorSpeed)
	{
		m_vectorSpeed = vectorSpeed;
	}
	//----------------------------------------------------------------------------------------------------------
	void MainBall::OnHit(ICollidable& other)
	{
		OnInteracted();
	}
	//ISerializable
	//----------------------------------------------------------------------------------------------------------
	void MainBall::Serialize(std::ostream& out) const
	{
		out << m_scalarSpeed << " " << m_normalSpeed << " " << m_currentTime << " " << m_timerStarted << " " << m_vectorSpeed.x << " " << m_vectorSpeed.y << " " << m_position.x
			<< " " << m_position.y << " " << +m_sprite.getColor().a << " " << +m_sprite.getColor().r << " " << +m_sprite.getColor().g << " " << +m_sprite.getColor().b << "\n";
	}
	//----------------------------------------------------------------------------------------------------------
	void MainBall::Deserialize(std::istream& in)
	{
		sf::Color color;
		int colorA = 0;
		int colorR = 0;
		int colorG = 0;
		int colorB = 0;

		if ((in >> m_scalarSpeed >> m_normalSpeed >> m_currentTime >> m_timerStarted >> m_vectorSpeed.x >> m_vectorSpeed.y >> m_position.x >> m_position.y >> colorA >> colorR >> colorG >> colorB))
		{
			color.a = static_cast<sf::Uint8>(colorA);
			color.r = static_cast<sf::Uint8>(colorR);
			color.g= static_cast<sf::Uint8>(colorG);
			color.b= static_cast<sf::Uint8>(colorB);

			SetColor(color);
			if (m_timerStarted)
			{
				StartTimer(m_currentTime);
			}
		}
		else
		{
			throw std::runtime_error("Ошибка данных при чтении Ball");
		}
	}
	//Handlers
	//----------------------------------------------------------------------------------------------------------
	void MainBall::DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other)
	{
		if (std::dynamic_pointer_cast<PoisonBall>(observable) && observable.get() != dynamic_cast<GameObservable*>(&other) )
		{
			ChangeSpeed(true);
			StartTimer(3.0f);
		}
	}
	//Timer
	//----------------------------------------------------------------------------------------------------------
	void MainBall::FinalAction()
	{
		ChangeSpeed(false);
	}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::ChangeSpeed(bool up)
	{
		if (up)
		{
			m_scalarSpeed = m_poisonedSpeed;
			SetColor(sf::Color::Green);
		}
		else
		{
			m_scalarSpeed = m_normalSpeed;
			SetColor(sf::Color::White);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::CheckCollisions()
	{
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();
		if (gameState)
		{
			std::vector<std::shared_ptr<Block>> blocks = gameState->GetBlocks();
			std::shared_ptr<Base> base = gameState->GetBase();

			for (const auto block : blocks)			{
				if (Math::GetInstance().IsCicleRectangleCollition(m_position, m_size.x * 0.5f, block->GetPosition(), block->GetSize()))
				{
					SetVectorSpeed(Math::GetInstance().CalculateReboundSpeedByRectangle(m_position, m_size.x * 0.5f, m_vectorSpeed, block->GetPosition(), block->GetSize(), BASE_REBOUND_MAX_ANGLE));
					UpdateBallPosition(0.005f);
					OnHit(*this);
					block->OnHit(*this);
					return;
				}
			}

			if (Math::GetInstance().IsCicleRectangleCollition(m_position, m_size.x * 0.5f, base->GetPosition(), base->GetSize()))
			{
				SetVectorSpeed(Math::GetInstance().CalculateReboundSpeedByRectangle(m_position, m_size.x * 0.5f, m_vectorSpeed, base->GetPosition(), base->GetSize(), BASE_REBOUND_MAX_ANGLE));
				OnHit(*this);
				UpdateBallPosition(0.015f);
				return;
			}

			if (m_position.x - BALL_SIZE * 0.5f <= 0)
			{
				m_vectorSpeed = { abs(m_vectorSpeed.x),  m_vectorSpeed.y };
				OnInteracted();
			}
			else if (m_position.x + BALL_SIZE * 0.5f >= SCREEN_WIDTH)
			{
				m_vectorSpeed.x = abs(m_vectorSpeed.x);
				m_vectorSpeed = { m_vectorSpeed.x * (-1.0f),  m_vectorSpeed.y};
				OnInteracted();
			}
			else if (m_position.y - BALL_SIZE * 0.5f <= 0)
			{
				m_vectorSpeed = { m_vectorSpeed.x, abs(m_vectorSpeed.y) };
				OnInteracted();
			}
			else if (m_position.y >= SCREEN_HEIGHT)
			{
				OnDestracted(*this);
			}
		}
		else
		{
			std::cerr << "GameStateRunTimeNotValid: "  << std::endl;
		}
	}
	void MainBall::UpdateBallPosition(float deltaTime)
	{
		Ball::UpdateBallPosition(deltaTime);
		UpdateTimer(deltaTime);
	}
	//SpecialBall
	//------------------------------------------------------------------------------------------------------------
	SpecialBall::SpecialBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color) : Ball(size, position, vectorSpeed)
	{
		SetColor(color);
	}
	//------------------------------------------------------------------------------------------------------------
	void SpecialBall::OnHit(ICollidable& other)
	{
		OnDestracted(other);
	}
	//------------------------------------------------------------------------------------------------------------
	void SpecialBall::Serialize(std::ostream& out) const
	{
		out << m_scalarSpeed << " " << m_vectorSpeed.x << " " << m_vectorSpeed.y << " " << m_position.x << " " << m_position.y << "\n";
	}
	//------------------------------------------------------------------------------------------------------------
	void SpecialBall::Deserialize(std::istream& in)
	{
		in >> m_scalarSpeed >> m_vectorSpeed.x >> m_vectorSpeed.y >> m_position.x >> m_position.y;
	}
	//------------------------------------------------------------------------------------------------------------
	void SpecialBall::CheckCollisions()
	{
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();

		if (gameState)
		{
			std::shared_ptr<Base> base = gameState->GetBase();

			if (Math::GetInstance().IsCicleRectangleCollition(m_position, m_size.x * 0.5f, base->GetPosition(), base->GetSize()))
			{
				OnHit(*base);
				base->OnHit(*this);
			}

			if (m_position.x <= 0 || m_position.x >= SCREEN_WIDTH
				|| m_position.y <= 0 || m_position.y >= SCREEN_HEIGHT)
			{
				OnHit(*this);
			}
		}
		else
		{
			std::cerr << "GameStateRunTimeNotValid: " << std::endl;
		}
	}
}