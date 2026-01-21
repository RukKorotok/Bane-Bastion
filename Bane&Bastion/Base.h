#pragma once
#include "Objects.h"
#include "Interfaces.h"
#include "Observer.h"
#include "SmothTimer.h"

namespace Bane_Bastion
{
	class BaseSegment : public GameObject
	{

	public:

		BaseSegment(int index, bool edge);
		void SetIndex(int index);
		int GetIndex();
		void SetPosition(float xPos);
		void SetColor(sf::Color color);

	private:
		const float m_Y_POSITION_OFFSET = 50.0f;
		int m_index = 0;
	};

	class Base : public ICollidable, public GameObserver, public InputObserver, public SmothTimer, public ISerializable
	{
	public:

		Base();
		~Base();

		Vector2D GetPosition();
		Vector2D GetSize();
		void UpdateBase(float deltaTime, sf::RenderWindow& window);
		//ICollidable
		void OnHit(ICollidable& other) override {}
		//ISerializable
		virtual void Serialize(std::ostream& out) const;
		virtual void Deserialize(std::istream& in);
		//Timer
		void FinalAction() override;
		virtual void EachTickAction(float deltaTime) override {}
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override;
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {}
		void DoOnChangedMousePosition(float position) override;
		void DoOnInput(sf::Keyboard::Key key) override {}
		void DoOnInputText(sf::Uint32 unicode) override {}

	private:
		void UpdateBasePosition(float xPosition);
		void ChangeBaseColor(sf::Color color);
		void AddSegments();
		void SetDefault();

		const int m_START_SIZE = 3;

		std::vector<BaseSegment*> m_defaultSegments;
		std::vector<BaseSegment*> m_additionalSegments;
		int m_additionalSegmentsSize = 0;

		float m_xPosition = 0;
		const float m_TIMER_LENGHT = 6.0f;
	};
}
