#pragma once

#include "../Managers/CollisionsManager.h"
#include "../Models/Entity.h"

class EntityMovement
{
private:
	Entity* _entity;
	CollisionsManager* _collisions;
	Logger* _logger;

	bool _lockMovement;
	bool _noClip;

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
	void SetNoClip(bool noClip);
	void SetIdleStateName(std::string idle);
	void SetMoveStateName(std::string move);

	bool GetLockMovement();
	bool GetNoClip();
	std::string GetIdleStateName();
	std::string GetMoveStateName();

	void ToggleNoClip();
};

