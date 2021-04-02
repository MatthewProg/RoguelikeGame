#pragma once

#include "../Managers/CollisionsManager.h"
#include "../Models/Entity.h"

class EntityMovement
{
private:
	Entity* _entity;
	CollisionsManager* _collisions;
	Logger* _logger;
	Settings* _settings;

	bool _lockMovement;
	bool _noClip;

	std::string _idleState;
	std::string _moveState;

	sf::FloatRect GetNextHitboxPosition(float deltaTime);
public:
	EntityMovement();
	~EntityMovement();

	void Update(float deltaTime);
	bool IsKeyPressed() const;
	void MoveBy(float x, float y, float deltaTime);

	void SetEntity(Entity* entity);
	void SetCollisionsManager(CollisionsManager* manager);

	void SetLockMovement(bool lock);
	void SetNoClip(bool noClip);
	void SetIdleStateName(const std::string& idle);
	void SetMoveStateName(const std::string& move);

	bool GetLockMovement() const;
	bool GetNoClip() const;
	std::string GetIdleStateName() const;
	std::string GetMoveStateName() const;

	void ToggleNoClip();
};

