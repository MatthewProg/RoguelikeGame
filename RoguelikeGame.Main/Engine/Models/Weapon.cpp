#include "Weapon.h"

Weapon::Weapon(WeaponType type)
{
	_dmg = 0.5;
	_attackCooldown = 20;
	_cooldownCounter = 0;
	_currentAngle = 0;
	_isVisible = false;
	_showHitbox = false;
	_showRaycast = false;
	_weaponType = type;
	_raycastColor = sf::Color::Magenta;
	_raycast.setPrimitiveType(sf::PrimitiveType::Lines);
	_raycast.resize(2);
	_raycast[0].position = getPosition();
	_raycast[1].position = getPosition();
	_raycast[0].color = sf::Color::Magenta;
	_raycast[1].color = sf::Color::Magenta;
}

Weapon::Weapon(Weapon& other)
{
	_cooldownCounter = other._cooldownCounter;
	_weapon = other._weapon;
	_attackAnimation = other._attackAnimation;
	_currentAngle = other._currentAngle;
	_hitbox = other._hitbox;
	_raycast = other._raycast;
	_dmg = other._dmg;
	_attackCooldown = other._attackCooldown;
	_isVisible = other._isVisible;
	_showHitbox = other._showHitbox;
	_showRaycast = other._showRaycast;
	_hitboxColor = other._hitboxColor;
	_raycastColor = other._raycastColor;
	_weaponType = other._weaponType;
	
	setPosition(other.getPosition());
	setOrigin(other.getOrigin());
	setRotation(other.getRotation());
	setScale(other.getScale());
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

sf::Vector2f Weapon::GetRaycastHitpoint()
{
	return _raycast[1].position;
}

sf::Color Weapon::GetRaycastColor()
{
	return _raycastColor;
}

bool Weapon::GetRaycastVisibility() const
{
	return _showRaycast;
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

void Weapon::SetRaycastHitpoint(sf::Vector2f point)
{
	_raycast[0].position = getPosition();
	_raycast[1].position = point;
}

void Weapon::SetRaycastColor(sf::Color color)
{
	_raycastColor = color;
	_raycast[0].color = _raycastColor;
	_raycast[1].color = _raycastColor;
}

void Weapon::SetRaycastVisibility(bool visible)
{
	_showRaycast = visible;
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
