#pragma once

#include "TransformAnimation.h"
#include "Animation.h"

class Weapon : public sf::Transformable, public sf::Drawable
{
protected:
	unsigned int _cooldownCounter;
	sf::Animation _weapon;
	sf::TransformAnimation _attackAnimation;
	float _range;
	float _currentAngle;
	sf::VertexArray _hitbox;
private:
	float _dmg;
	unsigned short _attackCooldown;
	bool _isVisible;
	bool _showHitbox;
	sf::Color _hitboxColor;
public:
	Weapon();
	~Weapon();

	virtual bool CanAttack() = 0;
	virtual void Attack() = 0;
	virtual void Update(bool tick, float deltaTime) = 0;
	void ResetCooldown();

	float GetWeaponRange();
	float GetWeaponDMG();
	uint16_t GetWeaponCooldown();
	bool GetVisibility() const;
	float GetCurrentAngle();
	bool GetHitboxVisibility() const;
	sf::Color GetHitboxColor();
	sf::VertexArray GetHitbox();

	virtual void SetWeaponRange(float range) = 0;
	void SetWeaponDMG(float dmg);
	void SetWeaponCooldown(uint16_t cooldown);
	void SetVisibility(bool visibility);
	virtual void SetCurrentAngle(float angle) = 0;
	void SetHitboxVisibility(bool visibility);
	void SetHitboxColor(sf::Color color);

	sf::Animation* GetAnimation();
	sf::TransformAnimation* GetTransformAnimation();

	void SetAnimation(sf::Animation animation);
	void SetTransformAnimation(sf::TransformAnimation transformAnimation);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

