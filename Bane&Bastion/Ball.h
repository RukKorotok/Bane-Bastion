#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"
#include "Objects.h"
#include "Interfaces.h"
#include "Observer.h"
#include "SmothTimer.h"

namespace Bane_Bastion
{
	enum BallType
	{
		Primary = 0,
		PoisonAction,
		IncreasingAction,
	};

	class GameObject;

	class Ball : public GameObject, public ICollidable, public GameObservable, public GameObserver, public ISerializable
	{
	public:

		Ball(Vector2D size, Vector2D position, Vector2D vectorSpeed);
		~Ball() {}
		virtual void UpdateBall(float deltaTime, sf::RenderWindow& window);
		//ICollidable
		void OnHit(ICollidable& other) override {}
		//ISerializable
		virtual void Serialize(std::ostream& out) const {}
		virtual void Deserialize(std::istream& in) {}

	protected:

		virtual void CheckCollisions() {};
		virtual void UpdateBallPosition(float deltaTime);

		float m_scalarSpeed = 0.0f;
		Vector2D m_vectorSpeed = { 100.0f, 100.0f };
	};

	class MainBall final : public Ball, public SmothTimer

	{
	public:

		MainBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color);
		~MainBall() {}
;
		void SetPosition(Vector2D position);
		void SetVectorSpeed(Vector2D vectorSpeed);
		//ICollidable
		void OnHit(ICollidable& other) override;
		//ISerializable
		virtual void Serialize(std::ostream& out) const;
		virtual void Deserialize(std::istream& in);
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override;
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {};
		void DoOnInput(sf::Keyboard::Key key) {}
		//Timer
		void FinalAction() override;
		void EachTickAction(float deltaTime) override {};

	private:
		void ChangeSpeed(bool up);
		void CheckCollisions() override;
		void UpdateBallPosition(float deltaTime) override;

		float m_normalSpeed = 0.0f;
		const float m_poisonedSpeed = 100.0f;
	};

	class SpecialBall : public Ball

	{
	public:

		SpecialBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color);
		~SpecialBall() {}

		//ICollidable
		void OnHit(ICollidable& other) override;
		//ISerializable
		virtual void Serialize(std::ostream& out) const;
		virtual void Deserialize(std::istream& in);
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override {}
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {}
		void DoOnInput(sf::Keyboard::Key key) {}

	private:

		void CheckCollisions() override;
	};

	class PoisonBall final : public SpecialBall

	{
	public:

		PoisonBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color) 
			: SpecialBall(size, position, vectorSpeed, color) {}
		~PoisonBall() {}

		//ICollidable
		void OnHit(ICollidable& other) override { SpecialBall::OnHit(other); }
		//ISerializable
		virtual void Serialize(std::ostream& out) const { SpecialBall::Serialize(out); }
		virtual void Deserialize(std::istream& in) { SpecialBall::Deserialize(in); }
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override {}
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {}
		void DoOnInput(sf::Keyboard::Key key) {}
	};

	class IncreasingBall final : public SpecialBall

	{
	public:

		IncreasingBall(Vector2D size, Vector2D position, Vector2D vectorSpeed, sf::Color color)
			: SpecialBall(size, position, vectorSpeed, color) {}
		~IncreasingBall() {}

		//ICollidable
		void OnHit(ICollidable& other) override { SpecialBall::OnHit(other); }
		//ISerializable
		virtual void Serialize(std::ostream& out) const { SpecialBall::Serialize(out); }
		virtual void Deserialize(std::istream& in) { SpecialBall::Deserialize(in); }
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override {}
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {}
		void DoOnInput(sf::Keyboard::Key key) {}

	};


}