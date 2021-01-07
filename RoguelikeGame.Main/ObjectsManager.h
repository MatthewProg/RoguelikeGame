#pragma once

#include "TexturesManager.h"
#include "HitboxWeapon.h"
#include "MeleeWeapon.h"
#include "TilesHelper.h"
#include "Enemy.h"

class ObjectsManager
{
private:
	TexturesManager* _textures;

	std::map<std::string, HitboxWeapon*> _hitboxWeapons;
	std::map<std::string, MeleeWeapon*> _meleeWeapons;
	std::map<std::string, Enemy*> _enemies;

	//Melee weapons
	MeleeWeapon* CreateMeleeWeaponSword();

	//Hitbox weapons
	HitboxWeapon* CreateHitboxWeaponBite();

	//Enemies
	Enemy* CreateEnemyDevil();
public:
	MeleeWeapon* GetMeleeWeapon(std::string name);
	HitboxWeapon* GetHitboxWeapon(std::string name);
	Enemy* GetEnemy(std::string name);

	void SetTexturesManager(TexturesManager* textures);

	ObjectsManager();
	~ObjectsManager();
};

