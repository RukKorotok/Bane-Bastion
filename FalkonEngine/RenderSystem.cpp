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
		m_window = newWindow;
	}
	sf::RenderWindow& RenderSystem::GetMainWindow() const
	{
		return *m_window;
	}

	void RenderSystem::Render(const sf::Drawable& drawable)
	{
		m_window->draw(drawable);
	}
}