#pragma once
#include "../Models/Weapon.h"

class HitboxWeapon : public Weapon
{
private:
	std::vector<std::string> _hitSounds;

	// Inherited via Weapon
	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	HitboxWeapon();
	HitboxWeapon(HitboxWeapon& other);
	~HitboxWeapon();

	// Inherited via Weapon
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void Update(bool tick, float) override;
	virtual Weapon* clone() override;

	virtual void SetCurrentAngle(float) override;

	//Sound getters
	std::vector<std::string>* GetHitSounds();
};

