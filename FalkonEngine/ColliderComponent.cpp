#include "pch.h"
#include "ColliderComponent.h"

namespace FalkonEngine
{
	//ColliderComponent
	//-----------------------------------------------------------------------------------------------------------
	ColliderComponent::ColliderComponent(GameObject* gameObject) : Component(gameObject)
	{
		FE_CORE_ASSERT(p_gameObject != nullptr, "ColliderComponent created without a valid GameObject!");

		GameEvent event;
		event.type = FalkonEngine::GameEventType::SubObjectSpawned;
		event.sender = this;

		Notify(event);

		FE_APP_TRACE("ColliderComponent initialized for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SetTrigger(bool newIsTrigger)
	{
		m_isTrigger = newIsTrigger;
		FE_CORE_INFO("Collider on '" + p_gameObject->GetName() + "' set as trigger: " + (m_isTrigger ? "True" : "False"));
	}
	// -- (Subscribes) ---
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		if(onCollisionAction)
		{
			p_onCollisionActions.push_back(onCollisionAction);
		}
		else
		{
			FE_CORE_WARN("Attempted to subscribe an empty function to OnCollision in: " + p_gameObject->GetName());
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		auto targetPtr = onCollisionAction.target<void(Collision)>();
		if (!targetPtr)
		{
			FE_CORE_WARN("UnsubscribeCollision failed: target comparison impossible (possibly a lambda with capture) in: " + p_gameObject->GetName());
			return;
		}

		p_onCollisionActions.erase(std::remove_if(p_onCollisionActions.begin(), p_onCollisionActions.end(),
			[targetPtr](const std::function<void(Collision)>& action) {
				return action.target<void(Collision)>() == targetPtr;
			}), p_onCollisionActions.end());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		{
			if (onTriggerEnterAction) p_onTriggerEnterActions.push_back(onTriggerEnterAction);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		auto targetPtr = onTriggerEnterAction.target<void(Trigger)>();
		if (!targetPtr) return;

		p_onTriggerEnterActions.erase(std::remove_if(p_onTriggerEnterActions.begin(), p_onTriggerEnterActions.end(),
			[targetPtr](const std::function<void(Trigger)>& action) {
				return action.target<void(Trigger)>() == targetPtr;
			}), p_onTriggerEnterActions.end());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		{
			if (onTriggerExitAction) p_onTriggerExitActions.push_back(onTriggerExitAction);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		auto targetPtr = onTriggerExitAction.target<void(Trigger)>();
		if (!targetPtr) return;

		p_onTriggerExitActions.erase(std::remove_if(p_onTriggerExitActions.begin(), p_onTriggerExitActions.end(),
			[targetPtr](const std::function<void(Trigger)>& action) {
				return action.target<void(Trigger)>() == targetPtr;
			}), p_onTriggerExitActions.end());
	}
	//protected
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnCollision(Collision collision)
	{
		for (size_t i = 0; i < p_onCollisionActions.size(); ++i)
		{
			try
			{
				if (p_onCollisionActions[i])
				{
					p_onCollisionActions[i](collision);
				}
			}
			catch (const std::exception& e)
			{
				FE_CORE_ERROR("Exception in OnCollision for '" + p_gameObject->GetName() + "': " + e.what());
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnTriggerEnter(Trigger trigger)
	{
		for (size_t i = 0; i < p_onTriggerEnterActions.size(); ++i)
		{
			try
			{
				if (p_onTriggerEnterActions[i])
				{
					p_onTriggerEnterActions[i](trigger);
				}
			}
			catch (const std::exception& e)
			{
				FE_CORE_ERROR("Exception in OnTriggerEnter for '" + p_gameObject->GetName() + "': " + e.what());
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ColliderComponent::OnTriggerExit(Trigger trigger)
	{
		for (size_t i = 0; i < p_onTriggerExitActions.size(); ++i)
		{
			try
			{
				if (p_onTriggerExitActions[i])
				{
					p_onTriggerExitActions[i](trigger);
				}
			}
			catch (const std::exception& e)
			{
				FE_CORE_ERROR("Exception in OnTriggerExit for '" + p_gameObject->GetName() + "': " + e.what());
			}
		}
	}
}