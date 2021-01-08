#include "MeleeWeapon.h"

void MeleeWeapon::PrepareHitbox()
{
	float step = _angle / (float)_hitboxAccuracy;
	unsigned short p = 0;
	for (size_t i = 0; i < _hitbox.getVertexCount(); i += 3)
	{
		_hitbox[i].position = getOrigin();
		_hitbox[i + 1].position = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle - (_angle / 2) + (p * step), _range);
		_hitbox[i + 2].position = MathHelper::GetPointFromAngle(getOrigin(), _currentAngle - (_angle / 2) + ((p+1) * step), _range);
		p++;
	}
}

void MeleeWeapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	if (GetVisibility())
		target.draw(_weapon, states);
	if (GetHitboxVisibility())
		target.draw(_hitbox, states);
	if (GetRaycastVisibility())
		target.draw(_raycast);
}

MeleeWeapon::MeleeWeapon() : Weapon(WeaponType::MELEE)
{
	_range = 20;
	_hitboxAccuracy = 4;
	_currentAngle = 0;
	_cooldownCounter = 0;

	_angle = 60;
	_hitbox.setPrimitiveType(sf::PrimitiveType::Triangles);
	_hitbox.resize(12);
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

void MeleeWeapon::SetHitboxAccuracy(unsigned short steps)
{
	_hitboxAccuracy = steps;
	_hitbox.resize((size_t)_hitboxAccuracy * 3);
}



float MeleeWeapon::GetWeaponAngle()
{
	return _angle;
}

float MeleeWeapon::GetWeaponRange()
{
	return _range;
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
