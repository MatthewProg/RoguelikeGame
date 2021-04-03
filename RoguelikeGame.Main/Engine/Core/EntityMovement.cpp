#include "EntityMovement.h"

EntityMovement::EntityMovement()
{
	_logger = Logger::GetInstance();
	_settings = Settings::GetInstance();
	_collisions = nullptr;
	_entity = nullptr;
	_idleState = "idle";
	_moveState = "move";
	_lockMovement = false;
	_noClip = false;
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

		if (_collisions == nullptr || _noClip == true)
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

bool EntityMovement::IsKeyPressed() const
{
	return (sf::Keyboard::isKeyPressed(_settings->MOVE_UP) ||
		sf::Keyboard::isKeyPressed(_settings->MOVE_DOWN) ||
		sf::Keyboard::isKeyPressed(_settings->MOVE_LEFT) ||
		sf::Keyboard::isKeyPressed(_settings->MOVE_RIGHT));
}

sf::FloatRect EntityMovement::GetNextHitboxPosition(float deltaTime)
{
	if (_entity == nullptr) return sf::FloatRect();

	float moveX = 0;
	float moveY = 0;

	if (sf::Keyboard::isKeyPressed(_settings->MOVE_RIGHT))
		moveX += (_entity->GetStep() * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(_settings->MOVE_LEFT))
		moveX -= (_entity->GetStep() * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(_settings->MOVE_DOWN))
		moveY += (_entity->GetStep() * deltaTime * _entity->GetSpeed());
	if (sf::Keyboard::isKeyPressed(_settings->MOVE_UP))
		moveY -= (_entity->GetStep() * deltaTime * _entity->GetSpeed());

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

void EntityMovement::SetNoClip(bool noClip)
{
	_noClip = noClip;
}

void EntityMovement::SetIdleStateName(const std::string& idle)
{
	_idleState = idle;
}

void EntityMovement::SetMoveStateName(const std::string& move)
{
	_moveState = move;
}

bool EntityMovement::GetLockMovement() const
{
	return _lockMovement;
}

bool EntityMovement::GetNoClip() const
{
	return _noClip;
}

std::string EntityMovement::GetIdleStateName() const
{
	return _idleState;
}

std::string EntityMovement::GetMoveStateName() const
{
	return _moveState;
}

void EntityMovement::ToggleNoClip()
{
	std::string status = (!_noClip) ? "true" : "false";
	_logger->Log(Logger::LogType::DEBUG , "Toggle no clip: " + status);
	SetNoClip(!_noClip);
}
