#include "HitboxWeapon.h"

HitboxWeapon::HitboxWeapon() : Weapon(WeaponType::NONE)
{
}

HitboxWeapon::HitboxWeapon(HitboxWeapon& other) : Weapon(other)
{
}

HitboxWeapon::~HitboxWeapon()
{
}

bool HitboxWeapon::CanAttack()
{
    return (_cooldownCounter >= GetWeaponCooldown());
}

void HitboxWeapon::Attack()
{
    if (CanAttack() == false)
        return;

	_cooldownCounter = 0;
}

void HitboxWeapon::Update(bool tick, float)
{
	if (tick && _cooldownCounter < GetWeaponCooldown())
		_cooldownCounter++;

	if(_weapon.GetNoOfFrames() > 0)
		_weapon.Tick(tick);
}

void HitboxWeapon::SetCurrentAngle(float)
{
}

void HitboxWeapon::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if (GetRaycastVisibility())
		target.draw(_raycast);
}

