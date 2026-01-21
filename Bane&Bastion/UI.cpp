#include "UI.h"
#include "Game.h"
#include "Menu.h"

namespace Bane_Bastion
{	
	//UI
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawMenu(sf::RenderWindow& window)
	{
		for(int i = 0; i < m_currentMenuList.size(); i++)
		{
			m_currentMenuList[i]->DrawMenu(window, { float(i), 0.0f });
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitSprite(sf::Sprite& sprite, const sf::Texture& texture, float heightMulty, float widthMulty)
	{
		float mainMenueSpriteSize = SCREEN_HEIGHT * 0.9f;
		float textXIndentation = SCREEN_WIDTH - 400.0f;

		sprite.setTexture(texture);
		Math::GetInstance().SetSpriteSize(sprite, mainMenueSpriteSize, mainMenueSpriteSize);
		Math::GetInstance().SetSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
		sprite.setPosition(SCREEN_HEIGHT * heightMulty, SCREEN_WIDTH * widthMulty);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitShape(sf::RectangleShape& shape, float shapeSizeX, float shapeSizeY, float sizeMulty, const sf::Color& color, float height, float width)
	{
		float shapeSize = shapeSizeX * sizeMulty;

		shape.setOrigin(shapeSizeX * 0.45f, shapeSizeY * 0.45f);
		shape.setSize({ shapeSizeX * sizeMulty, shapeSizeY * sizeMulty });
		shape.setPosition(height, width);
		shape.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitText(sf::Text& text, const std::string title, const sf::Color& color, const float heigh, const float width)
	{
		text.setFont(m_font);
		text.setCharacterSize(TEXT_SIZE - 5);
		text.setString(title);
		text.setFillColor(color);
		text.setPosition(heigh, width);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawGameUI(Game& game, sf::RenderWindow& window)
	{
		m_gameText.setString(SCORE_TITLE + std::to_string(game.GetCurrentGameState()->GetScore()));
		window.draw(m_gameText);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetStringInText(sf::Text& text, std::string string)
	{
		text.setString(string);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetShapeColor(sf::RectangleShape& button, const sf::Color& color)
	{
		button.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetTextColor(sf::Text& text, const sf::Color& color)
	{
		text.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Menu> Bane_Bastion::UI::CreateMenu(std::string section, bool customTitle, std::string title)
	{
		if (section == "RecordList")
		{
			std::shared_ptr<Menu> menu = std::make_shared<RecordsMenu>(section, *m_reader, m_currentMenuList.size());
			m_currentMenuList.push_back(menu);
			Game::GetInstance().GetInputHandler()->AddObserver(m_currentMenuList.back());
			return menu;
		}
		else
		{
			std::shared_ptr<Menu> menu = std::make_shared<Menu>(section, *m_reader, m_currentMenuList.size(), customTitle, title);
			m_currentMenuList.push_back(menu);
			Game::GetInstance().GetInputHandler()->AddObserver(m_currentMenuList.back());
			if (section == "GameOverMenu" && RecordsManager::GetInstance().IsEligible(Game::GetInstance().GetCurrentGameState()->GetScore()))
			{
				m_currentMenuList.push_back(std::make_shared<InputMenu>("InputMenu", *m_reader, m_currentMenuList.size()));
				Game::GetInstance().GetInputHandler()->AddObserver(m_currentMenuList.back());
			}
			return menu;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RemoveMenu(Menu& menu)
	{
		int i = 0;
		Menu* menuAdress = &menu;
		for (i = 0; i < m_currentMenuList.size(); i++)
		{		
			if (m_currentMenuList[i].get() == menuAdress)
			{
				m_currentMenuList.erase(m_currentMenuList.begin() + i);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Menu>> UI::GetMenu()
	{
		return m_currentMenuList;
	}
	//-----------------------------------------------------------------------------------------------------------
	INIReader UI::GetReader()
	{
		return *m_reader;
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RemoveMenu()
	{
		m_currentMenuList.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::UI()
	{
		m_reader = new INIReader("../Config/Menu.ini");

		if (m_reader->ParseError() < 0)
		{
			std::cerr << "Menu.ini not found" << std::endl;
		}

		assert(m_mainMenueTexture.loadFromFile(RESOURCES_PATH + "\\GameStart.png"));
		assert(m_font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		int i = 0;
		float mainMenueSpriteSize = SCREEN_HEIGHT * 0.9f;
		float menuIndentation = SCREEN_WIDTH - 250.0f;

		// Init UI sprites
		InitSprite(m_mainMenuSprite, m_mainMenueTexture, 0.38f, 0.35f);

		// Init UI text
		InitText(m_gameText, SCORE_TITLE, sf::Color::Yellow, SCREEN_WIDTH - 100.0f, 5.0f);
		InitText(m_nameInputText, INPUT_TITLE, sf::Color::Yellow, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f);
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::~UI()
	{
		m_currentMenuList.clear();
	}
}
