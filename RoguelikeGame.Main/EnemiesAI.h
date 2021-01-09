#pragma once

#include "CollisionsManager.h"
#include "EnemiesManager.h"
#include "ViewHelper.h"

class EnemiesAI
{
private:
	Entity* _target;
	EnemiesManager* _enemies;
	CollisionsManager* _collisions;

	bool DirectLineOfSight(Enemy* source, sf::Vector2f* raycastHitpoint);
public:
	EnemiesAI();
	~EnemiesAI();

	void Update(float deltaTime);
	void MoveStraightToPoint(Enemy* source, sf::Vector2f point, float deltaTime);

	void SetTarget(Entity* target);
	void SetEnemiesManager(EnemiesManager* manager);
	void SetCollisionsManager(CollisionsManager* manager);
};

