#include "Enemy.h"

Enemy::Enemy()
{
	//Collision changes
	SetHitboxColor(sf::Color(64, 255, 64, 64));
	SetHitboxOutlineColor(sf::Color(64, 255, 64, 160));
	SetHitboxOutlineThickness(-1.F);
	SetHitboxVisibility(false);

	//Entity changes
	SetHealth(.5F);
	SetSpeed(2.F);
	SetVisibility(true);

	_weapon = nullptr;
}

Enemy::~Enemy()
{
	if (_weapon != nullptr)
		delete _weapon;
}

void Enemy::Update(bool tick)
{
	Entity::UpdateEntity(tick);
}

Weapon* Enemy::GetWeapon()
{
	return _weapon;
}

void Enemy::SetWeapon(Weapon* weapon)
{
	if (_weapon != nullptr)
		delete _weapon;

	_weapon = weapon;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	if (_weapon != nullptr)
		_weapon->draw(target, states);
}