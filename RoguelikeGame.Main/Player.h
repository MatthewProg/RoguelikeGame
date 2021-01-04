#pragma once

#include "AnimationContainer.h"
#include "CollisionHelper.h"
#include "MapLayerModel.h"
#include "ViewHelper.h"
#include "Collision.h"
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
	void UpdateMovement(float delta, MapLayerModel<uint8_t>* tiles, uint8_t blockId);

	//Player getters
	sf::FloatRect GetNextHitboxPosition(float deltaTime);

	//Player setters
	void MoveBy(float x, float y, float deltaTime);

	void ToggleHitboxVisibility();
	void ToggleWeaponHitboxVisibility();

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
};

