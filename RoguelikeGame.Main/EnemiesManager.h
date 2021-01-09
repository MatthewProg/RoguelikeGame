#pragma once

#include "Logger.h"

#include "CollisionsManager.h"
#include "CollisionHelper.h"
#include "MeleeWeapon.h"
#include "Player.h"
#include "Enemy.h"

class EnemiesManager : public sf::Drawable
{
private:
	Logger* _logger;

	std::vector<Enemy*> _enemies;

	Player* _player;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	EnemiesManager();
	~EnemiesManager();

	void Update(bool tick, float deltaTime);
	void CheckForHit();
	void CheckAttacks();

	void SetPlayer(Player* player);
	void SetEnemiesHitboxVisibility(bool visibility);

	bool GetEnemiesHitboxVisibility();
	void ToggleEnemiesHitboxVisibility();
	void ToggleEnemiesRaycastVisibility();

	void Add(Enemy* enemy);
	std::vector<Enemy*>* GetEnemies();

};

