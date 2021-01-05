#pragma once

#include "CollisionsManager.h"
#include "Entity.h"

class EntityMovement
{
private:
	Entity* _entity;
	CollisionsManager* _collisions;

	bool _lockMovement;

	std::string _idleState;
	std::string _moveState;

	sf::FloatRect GetNextHitboxPosition(float deltaTime);
public:
	EntityMovement();
	~EntityMovement();

	void Update(float deltaTime);
	bool IsKeyPressed();
	void MoveBy(float x, float y, float deltaTime);

	void SetEntity(Entity* entity);
	void SetCollisionsManager(CollisionsManager* manager);

	void SetLockMovement(bool lock);
	void SetIdleStateName(std::string idle);
	void SetMoveStateName(std::string move);

	bool GetLockMovement();
	std::string GetIdleStateName();
	std::string GetMoveStateName();
};

