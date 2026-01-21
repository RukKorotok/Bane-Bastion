#include "Objects.h"
#include "Base.h"

namespace Bane_Bastion
{
	//GameObject
	//-----------------------------------------------------------------------------------------------------------
	GameObject::GameObject(std::string texturePath, Vector2D size, Vector2D position)
	{
		assert(m_texture.loadFromFile(texturePath) );

		m_sprite.setTexture(m_texture);
		Math::GetInstance().SetSpriteSize(m_sprite, size.x, size.y);
		Math::GetInstance().SetSpriteRelativeOrigin(m_sprite, 0.5f, 0.5f);
		m_sprite.setPosition(position.x, position.y);
		m_size = size;
		m_position = position;
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2D GameObject::GetPosition()
	{
		return m_position;
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2D GameObject::GetSize()
	{
		return m_size;
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Sprite GameObject::GetSprite()
	{
		return m_sprite;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Draw(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::SetColor(sf::Color color)
	{
		m_sprite.setColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Visualize(sf::RenderWindow& window)
	{
		Draw(window);
	}
	//MenuItem
	//-----------------------------------------------------------------------------------------------------------
	MenuItem::MenuItem(std::string text)
	{
		m_string = text;
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItem::Draw(std::string string, sf::Color color, sf::RenderWindow& window, Vector2D tabIndex)
	{
		UI::GetInstance().InitText(m_text, string, color, m_MENU_ITEMS_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_ITEMS_INDETATION.y + 50.0f * tabIndex.y);
		window.draw(m_text);
	}
	void MenuItem::Visualize(sf::RenderWindow& window, Vector2D tabIndex)
	{
		Draw(m_string, sf::Color::White, window, tabIndex);
	}
	std::string MenuItem::GetString()
	{
		return m_string;
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItem::ChangeText(std::string newText)
	{
		m_string = newText;
		m_text.setString(newText);
	}
}