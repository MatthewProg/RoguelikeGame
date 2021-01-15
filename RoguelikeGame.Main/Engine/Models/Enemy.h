#pragma once

#include "../Models/Weapon.h"
#include "../Models/Entity.h"

class Enemy : public Entity
{
private:
	Weapon* _weapon;

	float _tmpSpeed;
	bool _inAttack;
	bool _tmpStop;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Enemy();
	~Enemy();

	void Update(bool tick, float delta);
	void Attack();

	void SetTmpStop(bool toggle);

	bool GetTmpStop();
	bool IsAttacking();

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
};