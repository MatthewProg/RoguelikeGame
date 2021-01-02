#pragma once

#include "MeleeWeapon.h"
#include "TexturesManager.h"

class ObjectsManager
{
private:
	TexturesManager* _textures;

	std::map<std::string, MeleeWeapon*> _meleeWeapons;

	MeleeWeapon* CreateMeleeWeaponSword();
public:
	MeleeWeapon* GetMeleeWeapon(std::string name);

	void SetTexturesManager(TexturesManager* textures);

	ObjectsManager();
	~ObjectsManager();
};

