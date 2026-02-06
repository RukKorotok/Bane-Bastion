#pragma once

#include <SFML/Graphics.hpp>

namespace FalkonEngine
{
	class RenderSystem
	{
	public:
		static RenderSystem* Instance();

		void SetMainWindow(sf::RenderWindow* newWindow);
		sf::RenderWindow& GetMainWindow() const;

		void Render(const sf::Drawable& drawable);

	private:
		sf::RenderWindow* m_window = nullptr;

		RenderSystem() {}
		~RenderSystem() {}

		RenderSystem(RenderSystem const&) = delete;
		RenderSystem& operator= (RenderSystem const&) = delete;
	};
}