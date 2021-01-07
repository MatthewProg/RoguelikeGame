#include "HitboxWeapon.h"

HitboxWeapon::HitboxWeapon() : Weapon(WeaponType::NONE)
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

void HitboxWeapon::Update(bool tick, float deltaTime)
{
	if (tick && _cooldownCounter < GetWeaponCooldown())
		_cooldownCounter++;

	if(_weapon.GetNoOfFrames() > 0)
		_weapon.Tick(tick);
}

void HitboxWeapon::SetCurrentAngle(float angle)
{
}

void HitboxWeapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

