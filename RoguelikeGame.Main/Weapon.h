#pragma once

#include "TransformAnimation.h"
#include "Animation.h"

enum class WeaponType
{
	NONE, MELEE, PROJECTILE, HITSCAN
};

class Weapon : public sf::Transformable, public sf::Drawable
{
protected:
	unsigned int _cooldownCounter;

	sf::Animation _weapon;
	sf::TransformAnimation _attackAnimation;

	float _currentAngle;

	sf::VertexArray _hitbox;
	sf::VertexArray _raycast;
private:
	float _dmg;
	unsigned short _attackCooldown;

	bool _isVisible;
	bool _showHitbox;
	bool _showRaycast;

	sf::Color _hitboxColor;
	sf::Color _raycastColor;


	WeaponType _weaponType;
public:
	Weapon(WeaponType type);
	~Weapon();

	virtual bool CanAttack() = 0;
	virtual void Attack() = 0;
	virtual void Update(bool tick, float deltaTime) = 0;
	void ResetCooldown();

	float GetWeaponDMG();
	uint16_t GetWeaponCooldown();
	bool GetVisibility() const;
	float GetCurrentAngle();
	bool GetHitboxVisibility() const;
	sf::Color GetHitboxColor();
	std::vector<sf::Vector2f> GetHitbox();
	WeaponType GetWeaponType();
	sf::Vector2f GetRaycastHitpoint();
	sf::Color GetRaycastColor();
	bool GetRaycastVisibility() const;

	void SetWeaponDMG(float dmg);
	void SetWeaponCooldown(uint16_t cooldown);
	void SetVisibility(bool visibility);
	virtual void SetCurrentAngle(float angle) = 0;
	void SetHitboxVisibility(bool visibility);
	void SetHitboxColor(sf::Color color);
	void SetRaycastHitpoint(sf::Vector2f point);
	void SetRaycastColor(sf::Color color);
	void SetRaycastVisibility(bool visible);

	sf::Animation* GetAnimation();
	sf::TransformAnimation* GetTransformAnimation();

	void SetAnimation(sf::Animation animation);
	void SetTransformAnimation(sf::TransformAnimation transformAnimation);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

