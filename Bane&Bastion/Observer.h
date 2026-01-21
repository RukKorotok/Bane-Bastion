#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include "Interfaces.h"

namespace Bane_Bastion
{
	class Observer;
	class GameObserver;
	class InputObserver;

	template <typename T>
	struct target_class;

	template <typename C, typename R, typename... A>
	struct target_class<R(C::*)(A...)> { using type = C; };


	class Observable
	{
	public:
		virtual ~Observable() = default;
		void AddObserver(std::weak_ptr<Observer> observer);
		std::vector<std::weak_ptr<Observer>> GetObservers() const;
		void SetSelfObservable(std::shared_ptr<Observable> self);
		std::weak_ptr<Observable> GetSelfObservable() const;



	protected:
		template <typename Func, typename... Args>
		inline void CallObservers(Func func, Args... args)
		{
			using TClass = typename target_class<Func>::type;
			std::vector<std::shared_ptr<TClass>> activeObservers;

			auto it = std::remove_if(m_Observers.begin(), m_Observers.end(), [&](const auto& observer)
				{
					auto locked = observer.lock();
					if (!locked) {
						return true;
					}

					auto target = std::dynamic_pointer_cast<TClass>(locked);
					if (target) {
						activeObservers.push_back(target);
					}
					return false; 
				});

			if (it != m_Observers.end()) {
				m_Observers.erase(it, m_Observers.end());
			}

			for (auto& target : activeObservers)
			{
				((*target).*func)(args...);
			}
		}

		void OnInput(sf::Keyboard::Key key);

	private:
		std::vector<std::weak_ptr<Observer>> m_Observers;
		std::weak_ptr<Observable> m_self;

	};

	class Observer : public std::enable_shared_from_this<Observer>
	{
	public:
		virtual ~Observer() = default;
		std::weak_ptr<Observer> GetObserverRef();
		virtual void DoOnInput(sf::Keyboard::Key key) = 0;
	};

	class InputObservable : public Observable
	{

	protected:
		void OnChangedMousePosition(float position);
		void OnInputText(sf::Uint32 unicode);

	};

	class InputObserver : public Observer
	{
	public:
		virtual ~InputObserver() = default;
		void DoOnInput(sf::Keyboard::Key key) override {}
		virtual void DoOnChangedMousePosition(float position) = 0;
		virtual void DoOnInputText(sf::Uint32 unicode) = 0;
	};

	class GameObservable : public Observable
	{
	protected:
		virtual void OnDestracted(ICollidable& other);
		virtual void OnInteracted();

	private:
		std::vector<std::weak_ptr<GameObserver>> m_Observers;

	};

	class GameObserver : public Observer
	{
	public:
		virtual ~GameObserver() = default;
		void DoOnInput(sf::Keyboard::Key key) override {}
		virtual void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) = 0;
		virtual void DoOnInteracted(std::shared_ptr<GameObservable> observable) = 0;
	};
}