#include "HitboxWeapon.h"

HitboxWeapon::HitboxWeapon() : Weapon(WeaponType::NONE)
{
	_hitSounds.clear();
}

HitboxWeapon::HitboxWeapon(HitboxWeapon& other) : Weapon(other)
{
	_hitSounds = other._hitSounds;
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

	if (_hitSounds.size() > 0)
	{
		auto rng = rand() % _hitSounds.size();
		_sounds->PlaySoundIndependent(_hitSounds[rng]);
	}
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

std::vector<std::string>* HitboxWeapon::GetHitSounds()
{
	return &_hitSounds;
}

Weapon* HitboxWeapon::clone()
{
	return new HitboxWeapon(*this);
}

void HitboxWeapon::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if (GetRaycastVisibility())
		target.draw(_raycast);
}

