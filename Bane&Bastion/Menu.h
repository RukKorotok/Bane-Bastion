#pragma once
#include "Game.h"
#include "Objects.h"

namespace Bane_Bastion
{
	class RecordItem : public MenuItem
	{

	public:

		RecordItem(std::string name, int score);

		void Visualize(sf::RenderWindow& window, Vector2D tabIndex) override;
		int GetScore();

	private:

		int m_score = 0;
	};

	class MenuItemWithAction : public MenuItem
	{
	public:

		MenuItemWithAction(std::string text, std::string action);

		void Visualize(sf::RenderWindow& window, Vector2D tabIndex) override;
		virtual void ActionHandle();
		void SetSelection(bool selected);
		std::string GetAction();
		bool GetSelected();
		//Handler
		void DoOnInput(sf::Keyboard::Key key) override;

	protected:

		std::string m_action = "";
		bool m_selected = false;
	};

	class MenuItemToOpenSubmenu final : public MenuItemWithAction

	{
	public:

		MenuItemToOpenSubmenu(std::string text, std::string action);

		void ActionHandle() override;
	};

	class MenuItemToSetState final : public MenuItemWithAction
	{
	public:

		MenuItemToSetState(std::string text, std::string mainAction, std::string secondAction, std::string attribute);

		void Visualize(sf::RenderWindow& window, Vector2D tabIndex) override;
		void ActionHandle() override;

	private:

		const std::map<std::string, SettingsMode> m_SETTINGS_MAP = {
			{"SetSound", SoundOn},
			{"SetMusic", MusicOn}
		};
		const std::map<std::string, DifficultyLevel> m_DIFFICULTY_MAP = {
			{"SetEasy", Easy},
			{"SetMedium", Medium},
			{"SetHard", Hard}
		};
		const std::map<std::string, State> m_STATE_ACTIONS_MAP = {
			{"StartGame", State::GameInProgress},
			{"Exit", State::ExitGame},
			{"Resume", State::ResumeGame},
			{"Main", State::Main},
			{">>", State::Load},
			{"<<", State::Save},
		};

		const Vector2D m_MENU_CHECKBOX_INDETATION = { SCREEN_WIDTH - 50.0f, 110.0f };
		const float m_CHECKBOX_SIZE = 30.0f;
		std::string m_atribute = "";
		bool m_isHaveCheckBox = false;
		bool m_isOpenSubMenu = false;
		sf::RectangleShape m_checkBox;
		sf::RectangleShape m_checkBoxFrame;
		std::string m_secondAction = "";
	};

	class Menu : public InputObserver
	{
	public:
		Menu(const std::string& section, const INIReader& reader, int index, bool customTitle, std::string title);
		~Menu();

		virtual void DrawMenu(sf::RenderWindow& window, Vector2D tabMenu);
		std::stack<std::map<int, std::shared_ptr<MenuItemWithAction>>> GetStack();
		void AddMenuItems(const std::string& section, const INIReader& reader, bool customTitle, std::string title);
		void RemoveMenuStackItem();
		int GetMenuPointer();
		void SetMenuPointer(int pointer);
		std::map<int, std::shared_ptr<MenuItemWithAction>> GetMenuActiveItemList();
		//Handler
		void DoOnChangedMousePosition(float position) override {}
		void DoOnInput(sf::Keyboard::Key key) override;
		void DoOnInputText(sf::Uint32 unicode) override {}


	protected:
		void SwitchMenuItem(bool down);
		virtual std::map<int, std::shared_ptr<MenuItemWithAction>> LoadMenuItems(const std::string& section, const INIReader& reader, bool customTitle, std::string title);

		std::stack<std::map<int, std::shared_ptr<MenuItemWithAction>>> m_menuStack;
		int m_menuPointer = 1;
		sf::Sprite m_menuSprite;
		int m_index = 0;
	};

	class RecordsMenu final : public Menu
	{
	public:

		RecordsMenu(const std::string& section, const INIReader& reader, int index) :
			Menu(section, reader, index, false, "") {
		}

		void DrawMenu(sf::RenderWindow& window, Vector2D tabMenu) override;
	};

	class InputMenu final : public Menu
	{
	public:
		InputMenu(const std::string& section, const INIReader& reader, int index) :
			Menu(section, reader, index, true, "") {
		}

		//Handlers
		void DoOnInput(sf::Keyboard::Key key) override;
		void DoOnInputText(sf::Uint32 unicode) override;

	private:
		std::string m_name;
	};
}