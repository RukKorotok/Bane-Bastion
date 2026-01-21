#pragma once
#include <string>
#include <cassert>
#include "Config.h"
#include "SFML/Graphics.hpp"
#include "Math.h"
#include "UI.h"
#include "Observer.h"

namespace Bane_Bastion
{

	class GameObject
	{
	public:
	
		GameObject(std::string texturePath, Vector2D size, Vector2D position);
		~GameObject() {}

		Vector2D GetPosition();
		Vector2D GetSize();
		sf::Sprite GetSprite();
		void SetColor(sf::Color color);
		virtual void Visualize(sf::RenderWindow& window);

	protected:
	
		void Draw(sf::RenderWindow& window);
		Vector2D m_position;
		Vector2D m_size;
		sf::Texture m_texture;
		sf::Sprite m_sprite;
	};

	class MenuItem : public InputObserver
	{
	public:

		MenuItem(std::string text);

		virtual void Visualize(sf::RenderWindow& window, Vector2D tabIndex);
		std::string GetString();
		void ChangeText(std::string newText);
		//Handlers
		void DoOnChangedMousePosition(float position) override {}
		void DoOnInput(sf::Keyboard::Key key) override {}
		void DoOnInputText(sf::Uint32 unicode) override {}

	protected:

		void Draw(std::string string, sf::Color color, sf::RenderWindow& window, Vector2D tabIndex);
		const Vector2D m_MENU_ITEMS_INDETATION = { SCREEN_WIDTH * 0.7f, 100.0f };
		const float MENU_INDENTATION_OFFSET = 120.0f;
		std::string m_string = "";
		sf::Text m_text;
	};
}