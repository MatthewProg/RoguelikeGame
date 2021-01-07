#pragma once
#include "Weapon.h"

class HitboxWeapon : public Weapon
{
private:
	// Inherited via Weapon
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	HitboxWeapon();
	~HitboxWeapon();

	// Inherited via Weapon
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void Update(bool tick, float deltaTime) override;

	virtual void SetCurrentAngle(float angle) override;
};

