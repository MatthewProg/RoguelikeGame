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
	bool _aiEnabled;

	float _avoidanceRadius;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Enemy();
	~Enemy() override;

	void Update(bool tick, float delta);
	void Attack();

	void SetTmpStop(bool toggle);
	void SetAI(bool enable);
	void SetAvoidanceRadius(float radius);

	bool GetTmpStop() const;
	bool IsAttacking() const;
	bool IsAiEnabled() const;
	float GetAvoidanceRadius() const;

	//Weapon
	Weapon* GetWeapon();
	void SetWeapon(Weapon* weapon);
};