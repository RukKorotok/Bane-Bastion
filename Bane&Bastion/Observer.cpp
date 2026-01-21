#include "Observer.h"

namespace Bane_Bastion
{
	//Observable
	void Observable::AddObserver(std::weak_ptr<Observer> observer)
	{
		m_Observers.push_back(observer);
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::weak_ptr<Observer>> Observable::GetObservers() const
	{
		return m_Observers;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Observable::SetSelfObservable(std::shared_ptr<Observable> self)
	{
		m_self = self;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::weak_ptr<Observable> Observable::GetSelfObservable() const
	{
		return m_self.lock();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Observable::OnInput(sf::Keyboard::Key key)
	{
		CallObservers(&Observer::DoOnInput, key);
	}
	//InputObservable
	//-----------------------------------------------------------------------------------------------------------
	void InputObservable::OnChangedMousePosition(float position)
	{
		CallObservers(&InputObserver::DoOnChangedMousePosition, position);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Bane_Bastion::InputObservable::OnInputText(sf::Uint32 unicode)
	{
		CallObservers(&InputObserver::DoOnInputText, unicode);
	}
	//GameObservable
	//-----------------------------------------------------------------------------------------------------------
	void GameObservable::OnDestracted(ICollidable& other)
	{
		if (auto self = GetSelfObservable().lock())
		{
			auto castedSelf = std::static_pointer_cast<GameObservable>(self);
			CallObservers(&GameObserver::DoOnDestracted, castedSelf, std::ref(other));
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObservable::OnInteracted()
	{
		if (auto self = GetSelfObservable().lock())
		{
			auto castedSelf = std::static_pointer_cast<GameObservable>(self);
			CallObservers(&GameObserver::DoOnInteracted, castedSelf);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::weak_ptr<Observer> Observer::GetObserverRef()
	{
		return std::weak_ptr<Observer>(shared_from_this());
	}
}