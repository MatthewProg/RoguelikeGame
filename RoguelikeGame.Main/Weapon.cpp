#include "Weapon.h"

Weapon::Weapon(WeaponType type)
{
	_dmg = 0.5;
	_attackCooldown = 20;
	_cooldownCounter = 0;
	_currentAngle = 0;
	_isVisible = false;
	_showHitbox = false;
	_weaponType = type;
}

Weapon::~Weapon()
{
}

void Weapon::ResetCooldown()
{
	_cooldownCounter = 0;
}

float Weapon::GetWeaponDMG()
{
	return _dmg;
}

uint16_t Weapon::GetWeaponCooldown()
{
	return _attackCooldown;
}

bool Weapon::GetVisibility() const
{
	return _isVisible;
}

float Weapon::GetCurrentAngle()
{
	return _currentAngle;
}

bool Weapon::GetHitboxVisibility() const
{
	return _showHitbox;
}

sf::Color Weapon::GetHitboxColor()
{
	return _hitboxColor;
}

std::vector<sf::Vector2f> Weapon::GetHitbox()
{
	auto &transform = getTransform();
	std::vector<sf::Vector2f> output;
	for (size_t i = 0; i < _hitbox.getVertexCount(); i++)
		output.push_back(transform.transformPoint(_hitbox[i].position));
	return output;
}

WeaponType Weapon::GetWeaponType()
{
	return _weaponType;
}

void Weapon::SetWeaponDMG(float dmg)
{
	_dmg = dmg;
}

void Weapon::SetWeaponCooldown(uint16_t cooldown)
{
	_attackCooldown = cooldown;
}

void Weapon::SetVisibility(bool visibility)
{
	_isVisible = visibility;
}

void Weapon::SetHitboxVisibility(bool visibility)
{
	_showHitbox = visibility;
}

void Weapon::SetHitboxColor(sf::Color color)
{
	_hitboxColor = color;

	for (size_t i = 0; i < _hitbox.getVertexCount(); i++)
		_hitbox[i].color = _hitboxColor;
}

sf::Animation* Weapon::GetAnimation()
{
	return &_weapon;
}

sf::TransformAnimation* Weapon::GetTransformAnimation()
{
	return &_attackAnimation;
}

void Weapon::SetAnimation(sf::Animation animation)
{
	_weapon = animation;
}

void Weapon::SetTransformAnimation(sf::TransformAnimation transformAnimation)
{
	_attackAnimation = transformAnimation;
}
