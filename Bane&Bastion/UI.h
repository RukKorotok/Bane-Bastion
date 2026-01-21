#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"
#include <stack>
#include "INIreader.h"

namespace Bane_Bastion
{

	enum SettingsMode
	{
		SoundOn = 1 << 0,
		MusicOn = 1 << 1,
	};

	enum DifficultyLevel
	{
		Easy = 0,
		Medium,
		Hard
	};

	enum State
	{
		Undefined = 0,
		GameInProgress,
		ResumeGame,
		Main,
		GameOver,
		Win,
		ExitGame,
		Load,
		Save
	};

	class Menu;
	class Game;

	class UI
	{
	public:

		static UI& GetInstance()
		{
			static UI ui;
			return ui;
		}

		void DrawMenu(sf::RenderWindow& window);
		void InitSprite(sf::Sprite& sprite, const sf::Texture& texture, float heightMulty, float widthMulty);
		void InitShape(sf::RectangleShape& shape, float shapeSizeX, float shapeSizeY, float sizeMulty, const sf::Color& color, float height, float width);
		void InitText(sf::Text& text, const std::string title, const sf::Color& color, const float height, const float width);
		void DrawGameUI(Game& game, sf::RenderWindow& window);
		void SetStringInText(sf::Text& text, std::string string);
		void SetShapeColor(sf::RectangleShape& shape, const sf::Color& color);
		void SetTextColor(sf::Text& text, const sf::Color& color);
		std::shared_ptr<Menu> CreateMenu(std::string section, bool customTitle, std::string title);
		void RemoveMenu(Menu& menu);
		std::vector<std::shared_ptr<Menu>> GetMenu();
		INIReader GetReader();
		void RemoveMenu();

	private:

		UI();
		~UI();

		UI(const UI&) = delete;
		UI& operator = (UI const&) = delete;

		std::vector<std::shared_ptr<Menu>> m_currentMenuList;
		sf::Font m_font;
		sf::Texture m_mainMenueTexture;
		sf::Sprite m_mainMenuSprite;
		sf::Text m_gameText;
		sf::Text m_nameInputText;
		bool m_drawPopUpMenu = false;
		std::string m_inputString = "";
		INIReader* m_reader;
	};
}
