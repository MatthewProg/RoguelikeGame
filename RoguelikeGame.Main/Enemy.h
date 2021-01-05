#pragma once

#include "Weapon.h"
#include "Entity.h"

class Enemy : public Entity
{
private:
	Weapon* _weapon;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Enemy();
	~Enemy();

	void Update(bool tick);
	//void UpdateMovement(float delta, MapLayerModel<uint8_t>* tiles, uint8_t blockId);

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
};