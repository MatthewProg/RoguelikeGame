#include "Enemy.h"

Enemy::Enemy()
{
	//Collision changes
	SetHitboxColor(sf::Color(64, 255, 64, 64));
	SetHitboxOutlineColor(sf::Color(64, 255, 64, 160));
	SetHitboxOutlineThickness(-.5F);
	SetHitboxVisibility(false);

	//Entity changes
	SetHealth(1.F);
	SetSpeed(1.F);
	SetVisibility(true);

	_tmpSpeed = 1.F;
	_inAttack = false;
	_tmpStop = false;
	_aiEnabled = false;
	_avoidanceRadius = 0.0F;

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
	if(_weapon != nullptr)
		_weapon->Update(tick, delta);

	if (GetHealth() <= 0)
	{
		_weapon = nullptr;
		SetSpeed(0.f);
	}

	if (_inAttack && GetState() != "attack")
	{
		SetTmpStop(false);
		_inAttack = false;
	}
}

void Enemy::Attack()
{
	SetState("attack");
	GetAnimations()->SmoothStateChange("idle");
	if (_weapon != nullptr)
		_weapon->Attack();

	SetTmpStop(true);
	_inAttack = true;
}

void Enemy::SetTmpStop(bool toggle)
{
	if (_tmpStop != toggle)
	{
		if (_tmpStop == false)
		{
			_tmpSpeed = GetSpeed();
			SetSpeed(0);
		}
		else
			SetSpeed(_tmpSpeed);
	}
	_tmpStop = toggle;
}

void Enemy::SetAI(bool enable)
{
	_aiEnabled = enable;
}

void Enemy::SetAvoidanceRadius(float radius)
{
	_avoidanceRadius = radius;
}

bool Enemy::GetTmpStop()
{
	return _tmpStop;
}

bool Enemy::IsAttacking()
{
	return _inAttack;
}

bool Enemy::IsAiEnabled()
{
	return _aiEnabled;
}

float Enemy::GetAvoidanceRadius()
{
	return _avoidanceRadius;
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