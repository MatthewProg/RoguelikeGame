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
	CollisionsManager* _collisionsManager; //DELETE when implementing enemies AI

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	EnemiesManager();
	~EnemiesManager();

	void Update(bool tick, float deltaTime);
	void CheckForHit();
	void CheckAttacks();
	void UpdateRays(); //DELETE when implementing enemies AI

	void SetPlayer(Player* player);
	void SetCollisionsManager(CollisionsManager* manager); //DELETE when implementing enemies AI
	void SetEnemiesHitboxVisibility(bool visibility);

	bool GetEnemiesHitboxVisibility();
	void ToggleEnemiesHitboxVisibility();
	void ToggleEnemiesRaycastVisibility();

	void Add(Enemy* enemy);
	std::vector<Enemy*>* GetEnemies();

};

