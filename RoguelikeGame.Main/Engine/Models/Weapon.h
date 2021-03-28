#pragma once

#include "../Utilities/TransformAnimation.h"
#include "../Utilities/Animation.h"
#include "../Managers/SoundsManager.h"

enum class WeaponType
{
	NONE, MELEE, PROJECTILE, HITSCAN
};

class Weapon : public sf::Transformable, public sf::Drawable
{
protected:
	unsigned int _cooldownCounter;

	sf::Animation _weapon;
	sf::TransformAnimation _attackAnimation;

	float _currentAngle;

	sf::VertexArray _hitbox;
	sf::VertexArray _raycast;

	SoundsManager* _sounds;
private:
	float _dmg;
	unsigned short _attackCooldown;

	bool _isVisible;
	bool _showHitbox;
	bool _showRaycast;

	sf::Color _hitboxColor;
	sf::Color _raycastColor;

	WeaponType _weaponType;
public:
	Weapon(WeaponType type);
	Weapon(Weapon& other);
	virtual ~Weapon() { ; }
	virtual Weapon* clone() = 0;

	virtual bool CanAttack() const = 0;
	virtual void Attack() = 0;
	virtual void Update(bool tick, float deltaTime) = 0;
	void ResetCooldown();

	float GetWeaponDMG() const;
	uint16_t GetWeaponCooldown() const;
	bool GetVisibility() const;
	float GetCurrentAngle() const;
	bool GetHitboxVisibility() const;
	const sf::Color& GetHitboxColor() const;
	std::vector<sf::Vector2f> GetHitbox() const;
	const WeaponType& GetWeaponType() const;
	const sf::Vector2f& GetRaycastHitpoint() const;
	const sf::Color& GetRaycastColor() const;
	bool GetRaycastVisibility() const;

	void SetWeaponDMG(float dmg);
	void SetWeaponCooldown(uint16_t cooldown);
	void SetVisibility(bool visibility);
	virtual void SetCurrentAngle(float angle) = 0;
	void SetHitboxVisibility(bool visibility);
	void SetHitboxColor(const sf::Color& color);
	void SetRaycastHitpoint(const sf::Vector2f& point);
	void SetRaycastColor(const sf::Color& color);
	void SetRaycastVisibility(bool visible);

	sf::Animation* GetAnimation();
	sf::TransformAnimation* GetTransformAnimation();

	void SetAnimation(const sf::Animation& animation);
	void SetTransformAnimation(const sf::TransformAnimation& transformAnimation);
	void SetSoundsManager(SoundsManager* manager);
		
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

