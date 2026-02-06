#pragma once
#include "GameWorld.h"

namespace FalkonEngine
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		static Scene* GetActive();
		GameWorld* GetWorld() const;

		virtual void Start() = 0;
		virtual void Restart() = 0;
		virtual void Stop() = 0;

	protected:
		GameWorld* m_world = nullptr;

	private:
		static inline Scene* s_activeScene = nullptr;
	};
}