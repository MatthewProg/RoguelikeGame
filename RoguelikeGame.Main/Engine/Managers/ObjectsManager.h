#pragma once

#include "../Handlers/ResultKeyHandler.hpp"
#include "../Managers/TexturesManager.h"
#include "../Managers/SoundsManager.h"
#include "../Managers/FontsManager.h"
#include "../Helpers/TilesHelper.h"
#include "../Helpers/UIHelper.h"
#include "../Models/HitboxWeapon.h"
#include "../Models/MeleeWeapon.h"
#include "../Models/Player.h"
#include "../Models/Enemy.h"
#include "../UI/FocusContainer.h"
#include "../UI/AnimationBox.h"
#include "../UI/ProgressBar.h"
#include "../UI/ListSelect.h"
#include "../UI/ScrollView.h"
#include "../UI/ScrollBar.h"
#include "../UI/CheckBox.h"
#include "../UI/Popup.hpp"
#include "../UI/Button.h"
#include "../UI/Label.h"
#include "../UI/Scene.h"

class ObjectsManager
{
private:
	TexturesManager* _textures;
	FontsManager* _fonts;
	SoundsManager* _sounds;
	Settings* _settings;
	sf::Vector2u _windowSize;
	sf::Event* _event;

	std::map<std::string, HitboxWeapon*> _hitboxWeapons;
	std::map<std::string, MeleeWeapon*> _meleeWeapons;
	std::map<std::string, Enemy*> _enemies;
	std::map<std::string, Player*> _players;

	std::map<std::string, std::pair<Scene*, std::string>> _popups;
	std::map<std::string, FocusContainer*> _focusContainers;
	std::map<std::string, ProgressBar*> _progressBars;
	std::map<std::string, ListSelect*> _listSelects;
	std::map<std::string, ScrollBar*> _scrollBars;
	std::map<std::string, CheckBox*> _checkBoxes;
	std::map<std::string, Button*> _buttons;
	std::map<std::string, Scene*> _scenes;


	//Melee weapons
	MeleeWeapon* CreateMeleeWeaponSword();

	//Hitbox weapons
	HitboxWeapon* CreateHitboxWeaponBite();

	//Enemies
	Enemy* CreateEnemyDevil();

	//Players
	Player* CreatePlayerMaleElf();

	//FocusContainers
	FocusContainer* CreateFocusContainerOptionBar();
	FocusContainer* CreateFocusContainerOptionCheckBox();
	FocusContainer* CreateFocusContainerOptionListSelect();
	FocusContainer* CreateFocusContainerOptionText();

	//ProgressBars
	ProgressBar* CreateProgressBarHeart();
	ProgressBar* CreateProgressBarOptions();

	//ListSelects
	ListSelect* CreateListSelectOptions();

	//ScrollBars
	ScrollBar* CreateScrollBarDefault();

	//CheckBoxes
	CheckBox* CreateCheckBoxDefault();

	//Buttons
	Button* CreateButtonDefaultRed();

	//Popups
	Scene* CreatePopupKey();

	//Scenes
	Scene* CreateSceneMainMenu();
	Scene* CreateSceneOptions();
	Scene* CreateSceneGameUI();

public:
	MeleeWeapon* GetMeleeWeapon(const std::string& name);
	HitboxWeapon* GetHitboxWeapon(const std::string& name);
	Enemy* GetEnemy(const std::string& name);
	Player* GetPlayer(const std::string& name);

	FocusContainer* GetFocusContainer(const std::string& name);
	ProgressBar* GetProgressBar(const std::string& name);
	ListSelect* GetListSelect(const std::string& name);
	ScrollBar* GetScrollBar(const std::string& name);
	CheckBox* GetCheckBox(const std::string& name);
	Button* GetButton(const std::string& name);
	Scene* GetPopup(const std::string& name);
	Scene* GetScene(const std::string& name);

	void SetTexturesManager(TexturesManager* textures);
	void SetFontsManager(FontsManager* fonts);
	void SetSoundsManager(SoundsManager* sounds);
	void SetWindowSize(const sf::Vector2u& size);
	void SetEvent(sf::Event* e);

	ObjectsManager();
	~ObjectsManager();
};

