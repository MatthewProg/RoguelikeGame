#pragma once

#include "../Helpers/ViewHelper.h"
#include "../Models/Entity.h"
#include "../Models/Weapon.h"

#include "../Core/Logger.h"

class Player : public Entity
{
private:
	Weapon* _weapon;
	Logger* _logger;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:

	Player();
	Player(Player& other);
	~Player() override;

	//Player
	void Update(bool tick, float delta);

	void ToggleHitboxVisibility();
	void ToggleWeaponHitboxVisibility();
	void ToggleRaycastVisibility();

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
	void SetWeaponUnsafe(Weapon* weapon);
};

