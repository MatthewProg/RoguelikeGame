#pragma once

#include "../Managers/TexturesManager.h"
#include "../Managers/SoundsManager.h"
#include "../Managers/FontsManager.h"
#include "../Helpers/TilesHelper.h"
#include "../Models/HitboxWeapon.h"
#include "../Models/MeleeWeapon.h"
#include "../Models/Player.h"
#include "../Models/Enemy.h"
#include "../UI/AnimationBox.h"
#include "../UI/ProgressBar.h"
#include "../UI/Button.h"
#include "../UI/Label.h"
#include "../UI/Scene.h"

class ObjectsManager
{
private:
	TexturesManager* _textures;
	FontsManager* _fonts;
	SoundsManager* _sounds;
	sf::Vector2u _windowSize;

	std::map<std::string, HitboxWeapon*> _hitboxWeapons;
	std::map<std::string, MeleeWeapon*> _meleeWeapons;
	std::map<std::string, Enemy*> _enemies;
	std::map<std::string, Player*> _players;

	std::map<std::string, ProgressBar*> _progressBars;
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

	//ProgressBars
	ProgressBar* CreateProgressBarHeart();

	//Buttons
	Button* CreateButtonDefaultRed();

	//Scenes
	Scene* CreateSceneMainMenu();
	Scene* CreateSceneGameUI();

public:
	MeleeWeapon* GetMeleeWeapon(std::string name);
	HitboxWeapon* GetHitboxWeapon(std::string name);
	Enemy* GetEnemy(std::string name);
	Player* GetPlayer(std::string name);

	ProgressBar* GetProgressBar(std::string name);
	Button* GetButton(std::string name);
	Scene* GetScene(std::string name);

	void SetTexturesManager(TexturesManager* textures);
	void SetFontsManager(FontsManager* fonts);
	void SetSoundsManager(SoundsManager* sounds);
	void SetWindowSize(sf::Vector2u size);

	ObjectsManager();
	~ObjectsManager();
};

