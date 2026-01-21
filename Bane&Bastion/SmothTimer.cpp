#include "SmothTimer.h"

namespace Bane_Bastion
{
	//------------------------------------------------------------------------------------------------------------
	void SmothTimer::StartTimer(float setedTime)
	{
		m_currentTime = setedTime;
		m_timerStarted = true;
	}
	//------------------------------------------------------------------------------------------------------------
	void SmothTimer::UpdateTimer(float deltaTime)
	{
		if (!m_timerStarted)
		{
			return;
		}
		m_currentTime -= deltaTime;
		EachTickAction(deltaTime);

		if (m_currentTime <= 0)
		{
			FinalAction();
			m_timerStarted = false;
		}
	}
}

