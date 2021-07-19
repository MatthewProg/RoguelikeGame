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
	~HitboxWeapon() override = default;

	// Inherited via Weapon
	bool CanAttack() const override;
	void Attack() override;
	void Update(bool tick, float) override;
	Weapon* clone() override;

	void SetCurrentAngle(float) override;

	//Sound getters
	std::vector<std::string>* GetHitSounds();
};

