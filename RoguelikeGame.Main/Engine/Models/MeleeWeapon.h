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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	MeleeWeapon();
	MeleeWeapon(MeleeWeapon& other);
	~MeleeWeapon();

	// Inherited via Weapon
	bool CanAttack() override;
	void Attack() override;
	void Update(bool tick, float deltaTime) override;
	virtual Weapon* clone() override;

	void SetHitboxAccuracy(unsigned short steps);

	//Weapon getters
	float GetWeaponAngle();
	float GetWeaponRange();

	//Weapon setteres
	void SetWeaponAngle(float angle);
	void SetWeaponRange(float range);
	void SetCurrentAngle(float angle) override;

	//Sound getters
	std::vector<std::string>* GetSwingSounds();
};

