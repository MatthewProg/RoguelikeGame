#pragma once

#include "ViewHelper.h"
#include "Entity.h"
#include "Weapon.h"

#include "Logger.h"

class Player : public Entity
{
private:
	Weapon* _weapon;
	Logger* _logger;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:

	Player();
	~Player();

	//Player
	void Update(bool tick, float delta);

	void ToggleHitboxVisibility();
	void ToggleWeaponHitboxVisibility();

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
};

