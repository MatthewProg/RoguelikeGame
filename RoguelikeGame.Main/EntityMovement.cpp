#include "EntityMovement.h"

EntityMovement::EntityMovement()
{
	_collisions = nullptr;
	_entity = nullptr;
	_idleState = "idle";
	_moveState = "move";
	_lockMovement = false;
}

EntityMovement::~EntityMovement()
{
}

void EntityMovement::Update(float deltaTime)
{
	if (_entity == nullptr || _lockMovement == true) return;

	if (IsKeyPressed())
	{
		_entity->SetState(_moveState);

		auto nextPos = GetNextHitboxPosition(deltaTime);
		auto currPos = _entity->GetCollisionBox();
		auto hitbox = _entity->GetCollisionBoxOffset();

		if (currPos.left > nextPos.left)
			_entity->GetAnimations()->ApplySetHorizontalFlip(true);
		else if (currPos.left < nextPos.left)
			_entity->GetAnimations()->ApplySetHorizontalFlip(false);

		if (_collisions == nullptr)
			_entity->SetPosition(sf::Vector2f(nextPos.left - hitbox.left, nextPos.top - hitbox.top));
		else
		{
			auto pos = _collisions->GetLimitPosition(currPos, nextPos);
			_entity->SetPosition(sf::Vector2f(pos.x - hitbox.left, pos.y - hitbox.top));
		}
	}
	else
		_entity->SetState(_idleState);
}

void EntityMovement::MoveBy(float x, float y, float deltaTime)
{
	if (_entity == nullptr) return;

	auto pos = _entity->GetPosition();
	_entity->SetPosition(pos.x + (x * deltaTime * _entity->GetSpeed()), pos.y + (y * deltaTime * _entity->GetSpeed()));
}

bool EntityMovement::IsKeyPressed()
{
	return (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
}

sf::FloatRect EntityMovement::GetNextHitboxPosition(float deltaTime)
{
	if (_entity == nullptr) return sf::FloatRect();

	float moveX = 0;
	float moveY = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		moveX += (2 * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		moveX -= (2 * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveY += (2 * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveY -= (2 * deltaTime * _entity->GetSpeed());

	auto rect = _entity->GetCollisionBox();
	return sf::FloatRect(rect.left + moveX, rect.top + moveY, rect.width, rect.height);
}

void EntityMovement::SetEntity(Entity* entity)
{
	_entity = entity;
}

void EntityMovement::SetCollisionsManager(CollisionsManager* manager)
{
	_collisions = manager;
}

void EntityMovement::SetLockMovement(bool lock)
{
	_lockMovement = lock;
}

void EntityMovement::SetIdleStateName(std::string idle)
{
	_idleState = idle;
}

void EntityMovement::SetMoveStateName(std::string move)
{
	_moveState = move;
}

bool EntityMovement::GetLockMovement()
{
	return _lockMovement;
}

std::string EntityMovement::GetIdleStateName()
{
	return _idleState;
}

std::string EntityMovement::GetMoveStateName()
{
	return _moveState;
}
