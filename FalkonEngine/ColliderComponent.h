#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Component.h"
#include "Collision.h"
#include "Trigger.h"
#include "PhysicsSystem.h"

namespace FalkonEngine
{
	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(GameObject* gameObject);

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;


		void SetTrigger(bool newIsTrigger);

		// -- (Subscribes) ---
		//** Added collition in collitions array */
		void SubscribeCollision(std::function<void(Collision)> onCollisionAction);

		//** Remove collition in collitions array */
		void UnsubscribeCollision(std::function<void(Collision)> onCollisionAction);

		//** Added enter trigger in collitions array */
		void SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

		//** Remove enter trigger in collitions array */
		void UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

		//** Added exit trigger in collitions array */
		void SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

		//** Remove exit trigger in collitions array */
		void UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

		friend class PhysicsSystem;

	protected:
		sf::FloatRect m_bounds;
		bool m_isTrigger = false;

		void OnCollision(Collision collision);
		void OnTriggerEnter(Trigger trigger);
		void OnTriggerExit(Trigger trigger);

		std::vector<std::function<void(Collision)>> p_onCollisionActions;
		std::vector<std::function<void(Trigger)>> p_onTriggerEnterActions;
		std::vector<std::function<void(Trigger)>> p_onTriggerExitActions;
	};
}