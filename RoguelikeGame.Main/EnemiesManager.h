#pragma once

#include "Logger.h"

#include "CollisionHelper.h"
#include "MeleeWeapon.h"
#include "Player.h"
#include "Enemy.h"

class EnemiesManager : public sf::Drawable
{
private:
	Logger* _logger;

	std::vector<Enemy*> _enemies;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	EnemiesManager();
	~EnemiesManager();

	void Update(bool tick, float deltaTime);
	void CheckForHit(Player* player);
	void CheckAttacks(Player* player);

	void SetEnemiesHitboxVisibility(bool visibility);
	bool GetEnemiesHitboxVisibility();
	void ToggleEnemiesHitboxVisibility();

	void Add(Enemy* enemy);
	std::vector<Enemy*>* GetEnemies();

};

