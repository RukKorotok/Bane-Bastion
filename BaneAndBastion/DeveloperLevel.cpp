#include "DeveloperLevel.h"

using namespace FalkonEngine;

namespace BaneAndBastion
{
	void DeveloperLevel::Start()
	{
		player = std::make_shared<Player>();
	}
	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}
	void DeveloperLevel::Stop() 
	{
		GameWorld::Instance()->Clear();
	}
}