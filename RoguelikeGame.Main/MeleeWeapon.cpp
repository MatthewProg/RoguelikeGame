#include "MeleeWeapon.h"

void MeleeWeapon::PrepareHitbox()
{
	auto lu = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle - (_angle / 2), _range);
	auto lm = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle - (_angle / 2), _range / 2);

	auto mu = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle, _range);
	auto mm = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle, _range / 2);

	auto ru = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle + (_angle / 2), _range);
	auto rm = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle + (_angle / 2), _range / 2);

	_hitbox[0].position = lu;
	_hitbox[1].position = mu;
	_hitbox[2].position = lm;
	_hitbox[3].position = mm;
	_hitbox[4].position = getOrigin();
	_hitbox[5].position = mm;
	_hitbox[6].position = rm;
	_hitbox[7].position = mu;
	_hitbox[8].position = ru;
}

void MeleeWeapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	if (GetVisibility())
		target.draw(_weapon, states);
	if (GetHitboxVisibility())
		target.draw(_hitbox, states);
}

MeleeWeapon::MeleeWeapon() : Weapon()
{
	_range = 20;
	_currentAngle = 0;
	_cooldownCounter = 0;

	_angle = 60;
	_hitbox.setPrimitiveType(sf::PrimitiveType::TrianglesStrip);
	_hitbox.resize(9);
	_attackAnimation.SetTarget(this);

	SetWeaponDMG(0.5);
	SetVisibility(false);
	SetWeaponCooldown(20);
	SetHitboxVisibility(false);
	SetHitboxColor(sf::Color(255, 0, 0, 64));
}

MeleeWeapon::~MeleeWeapon()
{
}

bool MeleeWeapon::CanAttack()
{
	return (_cooldownCounter >= GetWeaponCooldown());
}

void MeleeWeapon::Attack()
{
	if (CanAttack() == false)
		return;

	_cooldownCounter = 0;
	SetVisibility(true);
	PrepareHitbox();
	_attackAnimation.Reset();
	_attackAnimation.Start();
}

void MeleeWeapon::Update(bool tick, float deltaTime)
{
	if (tick && _cooldownCounter < GetWeaponCooldown())
		_cooldownCounter++;

	_attackAnimation.Update(deltaTime);
	_weapon.Tick(tick);

	if (_attackAnimation.IsEnded())
		SetVisibility(false);
}



float MeleeWeapon::GetWeaponAngle()
{
	return _angle;
}

void MeleeWeapon::SetWeaponRange(float range)
{
	_range = range;
	PrepareHitbox();
}

void MeleeWeapon::SetWeaponAngle(float angle)
{
	_angle = angle;
	PrepareHitbox();
}

void MeleeWeapon::SetCurrentAngle(float angle)
{
	_currentAngle = angle;
	_weapon.setRotation(angle + 90);
	if (GetHitboxVisibility())
		PrepareHitbox();
}
