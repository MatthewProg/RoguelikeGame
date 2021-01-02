#pragma once

#include "MathHelper.h"
#include "Weapon.h"

class MeleeWeapon : public Weapon
{
private:
	float _angle;
	void PrepareHitbox();

	// Inherited via Weapon::Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	MeleeWeapon();
	~MeleeWeapon();

	bool CanAttack();
	void Attack();
	void Update(bool tick, float deltaTime);

	//Weapon getters
	float GetWeaponAngle();

	//Weapon setteres
	void SetWeaponAngle(float angle);
	void SetWeaponRange(float range) override;
	void SetCurrentAngle(float angle) override;
};

