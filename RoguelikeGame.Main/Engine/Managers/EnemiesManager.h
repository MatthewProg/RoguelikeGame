#pragma once

#include "../Core/Logger.h"

#include "../Managers/CollisionsManager.h"
#include "../Helpers/CollisionHelper.h"
#include "../Models/MeleeWeapon.h"
#include "../Models/Player.h"
#include "../Models/Enemy.h"

class EnemiesManager : public sf::Drawable
{
private:
	Logger* _logger;

	std::vector<Enemy*> _enemies;

	Player* _player;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	EnemiesManager();
	~EnemiesManager();

	void Update(bool tick, float deltaTime);
	void CheckForHit();
	void CheckAttacks();

	void SetPlayer(Player* player);
	void SetEnemiesHitboxVisibility(bool visibility);

	bool GetEnemiesHitboxVisibility() const;
	void ToggleEnemiesHitboxVisibility();
	void ToggleEnemiesRaycastVisibility();

	void Add(Enemy* enemy);
	std::vector<Enemy*>* GetEnemies();
};

