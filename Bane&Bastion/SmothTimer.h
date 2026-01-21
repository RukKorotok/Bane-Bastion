#pragma once

namespace Bane_Bastion
{
	class SmothTimer
	{
	public:

		void StartTimer(float setedTime);
	
	protected:

		bool m_timerStarted = false;
		float m_currentTime = 0.0f;

		virtual void UpdateTimer(float deltaTime);

		virtual void FinalAction() = 0;
		virtual void EachTickAction(float deltaTime) = 0;
	};
}
