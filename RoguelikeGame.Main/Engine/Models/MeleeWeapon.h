#pragma once

#include "../Helpers/MathHelper.h"
#include "../Models/Weapon.h"

class MeleeWeapon : public Weapon
{
private:
	float _angle;
	float _range;

	unsigned short _hitboxAccuracy;

	void PrepareHitbox();

	std::vector<std::string> _swingSounds;

	// Inherited via Weapon::Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	MeleeWeapon();
	MeleeWeapon(MeleeWeapon& other);
	~MeleeWeapon() override = default;

	// Inherited via Weapon
	bool CanAttack() const override;
	void Attack() override;
	void Update(bool tick, float deltaTime) override;
	Weapon* clone() override;

	void SetHitboxAccuracy(unsigned short steps);

	//Weapon getters
	float GetWeaponAngle() const;
	float GetWeaponRange() const;

	//Weapon setteres
	void SetWeaponAngle(float angle);
	void SetWeaponRange(float range);
	void SetCurrentAngle(float angle) override;

	//Sound getters
	std::vector<std::string>* GetSwingSounds();
};

