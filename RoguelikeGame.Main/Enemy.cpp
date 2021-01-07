#include "Enemy.h"

Enemy::Enemy()
{
	//Collision changes
	SetHitboxColor(sf::Color(64, 255, 64, 64));
	SetHitboxOutlineColor(sf::Color(64, 255, 64, 160));
	SetHitboxOutlineThickness(-1.F);
	SetHitboxVisibility(false);

	//Entity changes
	SetHealth(1.F);
	SetSpeed(1.F);
	SetVisibility(true);

	_tmpSpeed = 1.F;
	_inAttack = false;

	_weapon = nullptr;
}

Enemy::~Enemy()
{
	if (_weapon != nullptr)
		delete _weapon;
}

void Enemy::Update(bool tick, float delta)
{
	Entity::UpdateEntity(tick);
	_weapon->Update(tick, delta);

	if (_inAttack && GetState() != "attack")
	{
		SetSpeed(_tmpSpeed);
		_inAttack = false;
	}
}

void Enemy::Attack()
{
	SetState("attack");
	GetAnimations()->SmoothStateChange("idle");
	_weapon->Attack();

	_tmpSpeed = GetSpeed();
	SetSpeed(0);
	_inAttack = true;
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