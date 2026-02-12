#pragma once
#include "GameWorld.h"
#include <string>

namespace FalkonEngine
{
	class Scene
	{
	public:
		Scene(const std::string& name);
		virtual ~Scene();
		static Scene* GetActive();
		GameWorld* GetWorld() const;
		std::string GetName();

		virtual void Start() = 0;
		virtual void Restart() = 0;
		virtual void Stop() = 0;

	protected:
		GameWorld* m_world = nullptr;

	private:
		static inline Scene* s_activeScene = nullptr;
		std::string m_name;

	};
}