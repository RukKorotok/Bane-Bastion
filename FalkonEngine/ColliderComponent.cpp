#include "pch.h"
#include "ColliderComponent.h"

namespace FalkonEngine
{
	//ColliderComponent
	//-----------------------------------------------------------------------------------------------------------
	ColliderComponent::ColliderComponent(GameObject* gameObject) : Component(gameObject)
	{
		GameEvent event;
		event.type = FalkonEngine::GameEventType::SubObjectSpawned;
		event.sender = this;

		Notify(event);
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SetTrigger(bool newIsTrigger)
	{
		m_isTrigger = newIsTrigger;
	}
	// -- (Subscribes) ---
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		p_onCollisionActions.push_back(onCollisionAction);
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		p_onCollisionActions.erase(std::remove_if
		(
			p_onCollisionActions.begin(),
			p_onCollisionActions.end(),
			[&onCollisionAction](const std::function<void(Collision)>& action)
			{
				return action.target<void(Collision)>() == onCollisionAction.target<void(Collision)>();
			}
		), p_onCollisionActions.end());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		p_onTriggerEnterActions.push_back(onTriggerEnterAction);
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		p_onTriggerEnterActions.erase(std::remove_if
		(
			p_onTriggerEnterActions.begin(),
			p_onTriggerEnterActions.end(),
			[&onTriggerEnterAction](const std::function<void(Trigger)>& action)
			{
				return action.target<void(Trigger)>() == onTriggerEnterAction.target<void(Trigger)>();
			}
		), p_onTriggerEnterActions.end());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		p_onTriggerExitActions.push_back(onTriggerExitAction);
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		p_onTriggerExitActions.erase(std::remove_if
		(
			p_onTriggerExitActions.begin(),
			p_onTriggerExitActions.end(),
			[&onTriggerExitAction](const std::function<void(Trigger)>& action)
			{
				return action.target<void(Trigger)>() == onTriggerExitAction.target<void(Trigger)>();
			}
		), p_onTriggerExitActions.end());
	}
	//protected
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnCollision(Collision collision)
	{
		for (int i = 0; i < p_onCollisionActions.size(); i++)
		{
			p_onCollisionActions[i](collision);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnTriggerEnter(Trigger trigger)
	{
		for (int i = 0; i < p_onTriggerEnterActions.size(); i++)
		{
			p_onTriggerEnterActions[i](trigger);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnTriggerExit(Trigger trigger)
	{
		for (int i = 0; i < p_onTriggerExitActions.size(); i++)
		{
			p_onTriggerExitActions[i](trigger);
		}
	}
}