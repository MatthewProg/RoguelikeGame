#pragma once

#include "../Managers/TexturesManager.h"
#include "../Helpers/TilesHelper.h"
#include "../Models/HitboxWeapon.h"
#include "../Models/MeleeWeapon.h"
#include "../Models/Player.h"
#include "../Models/Enemy.h"
#include "../UI/ProgressBar.h"

class ObjectsManager
{
private:
	TexturesManager* _textures;

	std::map<std::string, HitboxWeapon*> _hitboxWeapons;
	std::map<std::string, MeleeWeapon*> _meleeWeapons;
	std::map<std::string, Enemy*> _enemies;
	std::map<std::string, ProgressBar*> _progressBars;
	std::map<std::string, Player*> _players;

	//Melee weapons
	MeleeWeapon* CreateMeleeWeaponSword();

	//Hitbox weapons
	HitboxWeapon* CreateHitboxWeaponBite();

	//Enemies
	Enemy* CreateEnemyDevil();

	//ProgressBars
	ProgressBar* CreateProgressBarHeart();

	//Players
	Player* CreatePlayerMaleElf();
public:
	MeleeWeapon* GetMeleeWeapon(std::string name);
	HitboxWeapon* GetHitboxWeapon(std::string name);
	Enemy* GetEnemy(std::string name);
	ProgressBar* GetProgressBar(std::string name);
	Player* GetPlayer(std::string name);

	void SetTexturesManager(TexturesManager* textures);

	ObjectsManager();
	~ObjectsManager();
};

