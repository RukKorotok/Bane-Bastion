#include "pch.h"
#include "RenderSystem.h"

namespace FalkonEngine
{
	RenderSystem* RenderSystem::Instance()
	{
		static RenderSystem render;
		return &render;
	}

	void RenderSystem::SetMainWindow(sf::RenderWindow* newWindow)
	{
		if (newWindow == nullptr)
		{
			FE_CORE_ERROR("RenderSystem: Attempted to set a null RenderWindow!");
			return;
		}

		m_window = newWindow;
		FE_CORE_INFO("RenderSystem: Main window successfully attached.");
	}
	sf::RenderWindow& RenderSystem::GetMainWindow() const
	{
		FE_CORE_ASSERT(m_window != nullptr, "RenderSystem: Accessing MainWindow before initialization!");

		return *m_window;
	}

	void RenderSystem::Render(const sf::Drawable& drawable)
	{
		if (m_window != nullptr)
		{
			m_window->draw(drawable);
		}
		else
		{
			static bool windowErrorLogged = false;
			if (!windowErrorLogged)
			{
				FE_CORE_ERROR("RenderSystem: Cannot draw! MainWindow is null.");
				windowErrorLogged = true;
			}
		}
	}
}