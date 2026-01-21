#include "Base.h"
#include "Game.h"

namespace Bane_Bastion
{
	//-----------------------------------------------------------------------------------------------------------
	BaseSegment::BaseSegment(int index, bool edge) : 
		GameObject(edge == true ? RESOURCES_PATH + "PlatformEdge.png" : RESOURCES_PATH + "PlatformMiddle.png", { BASE_SEGMENT_SIZE, BASE_SEGMENT_SIZE }, { 0.0f, 0.0f })
	{
		if (index < 0)
		{
			Math::GetInstance().SetSpriteSize(m_sprite, BASE_SEGMENT_SIZE, BASE_SEGMENT_SIZE);
		}
		else
		{
			Math::GetInstance().SetSpriteSize(m_sprite, BASE_SEGMENT_SIZE * (- 1), BASE_SEGMENT_SIZE);
		}
		m_index = index;
		SetPosition(SCREEN_WIDTH * 0.5f);
	}
	//-----------------------------------------------------------------------------------------------------------
	void BaseSegment::SetIndex(int index)
	{
		m_index = index;
	}
	//-----------------------------------------------------------------------------------------------------------
	int BaseSegment::GetIndex()
	{
		return m_index;
	}
	//-----------------------------------------------------------------------------------------------------------
	void BaseSegment::SetPosition(float xPos)
	{
		m_position = { xPos + float(m_index) * BASE_SEGMENT_SIZE, SCREEN_HEIGHT - m_Y_POSITION_OFFSET };
		m_sprite.setPosition(m_position.x, m_position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	Base::Base()
	{
		BaseSegment* currentSegment;
		sf::Vector2f location;
		currentSegment = new BaseSegment(0, false);
		m_defaultSegments.push_back(currentSegment);
		currentSegment = new BaseSegment(1, true);
		m_defaultSegments.push_back(currentSegment);
		currentSegment = new BaseSegment(-1, true);
		m_defaultSegments.push_back(currentSegment);
	}
	//-----------------------------------------------------------------------------------------------------------
	Base::~Base()
	{
		for (BaseSegment* segment : m_defaultSegments)
		{
			delete segment;
		}
		m_defaultSegments.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2D Base::GetPosition()
	{
		return m_defaultSegments[0]->GetPosition();
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2D Base::GetSize()
	{
		return { BASE_SEGMENT_SIZE * (m_defaultSegments.size() + m_additionalSegments.size()), BASE_SEGMENT_SIZE };
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::UpdateBase(float deltaTime, sf::RenderWindow& window)
	{
		for (BaseSegment* currentSegment : m_defaultSegments)
		{
			currentSegment->Visualize(window);
		}
		for (BaseSegment* currentSegment : m_additionalSegments)
		{
			currentSegment->Visualize(window);
		}
		UpdateTimer(deltaTime);
	}
	//ISerializable
	//-----------------------------------------------------------------------------------------------------------
	void Base::Serialize(std::ostream& out) const
	{
		out << m_xPosition << " " << m_currentTime << " " << m_timerStarted << " " << m_additionalSegments.size() << "\n";
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::Deserialize(std::istream& in)
	{
		if (!(in >> m_xPosition >> m_currentTime >> m_timerStarted >> m_additionalSegmentsSize)) 
		{
			throw std::runtime_error("Ошибка данных при чтении Ball");
		}
	}
	//Timer
	//-----------------------------------------------------------------------------------------------------------
	void Base::FinalAction()
	{
		SetDefault();
		UpdateBasePosition(m_xPosition);
	}
	//Handlers
	//-----------------------------------------------------------------------------------------------------------
	void Base::DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other)
	{
		if (std::dynamic_pointer_cast<IncreasingBall>(observable) && observable.get() != dynamic_cast<GameObservable*>(&other))
		{
			AddSegments();
			UpdateBasePosition(m_xPosition);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::DoOnChangedMousePosition(float position)
	{
		UpdateBasePosition(position);
	}
	//Private
	//-----------------------------------------------------------------------------------------------------------
		void Base::UpdateBasePosition(float xPosition)
	{
		m_xPosition = xPosition;

		for (int i = 0; i < m_defaultSegments.size(); i++)
		{
			m_defaultSegments[i]->SetPosition(xPosition);
		}
		for (auto item : m_additionalSegments)
		{
			item->SetPosition(xPosition);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::ChangeBaseColor(sf::Color color)
	{
		if (m_defaultSegments[0]->GetSprite().getColor() != color)
		{
			for (int i = 0; i < m_defaultSegments.size(); i++)
			{
				m_defaultSegments[i]->GetSprite().setColor(color);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::AddSegments()
	{

		int backIndex = m_defaultSegments.back()->GetIndex();
		m_defaultSegments[m_defaultSegments.size() - 1]->SetIndex(backIndex - 1);
		m_defaultSegments[m_defaultSegments.size() - 2]->SetIndex(backIndex * -1 + 1);
		m_additionalSegments.push_back(new BaseSegment(backIndex * -1, false) );
		m_additionalSegments.push_back(new BaseSegment(backIndex, false) );
		StartTimer(m_TIMER_LENGHT);
		
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::SetDefault()
	{
		m_additionalSegments.clear();
		m_defaultSegments[m_defaultSegments.size() - 1]->SetIndex(- 1);
		m_defaultSegments[m_defaultSegments.size() - 2]->SetIndex(1);
	}
}
