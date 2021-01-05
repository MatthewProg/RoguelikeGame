#include "Entity.h"

void Entity::UpdateEntity(bool tick)
{
	_animations.Tick(tick);
}

Entity::Entity() : sf::Collision()
{
	_state = "";
	_health = 3.F;
	_speed = 1.F;
	_isVisible = true;
}

Entity::~Entity()
{
}

sf::AnimationContainer* Entity::GetAnimations()
{
	return &_animations;
}

std::string Entity::GetState()
{
	return _state;
}

float Entity::GetHealth()
{
	return _health;
}

float Entity::GetSpeed()
{
	return _speed;
}

bool Entity::GetVisibility()
{
	return _isVisible;
}

sf::Vector2f Entity::GetPosition()
{
	return _transform.getPosition();
}

float Entity::GetRotation()
{
	return _transform.getRotation();
}

sf::Vector2f Entity::GetOrigin()
{
	return _transform.getOrigin();
}

sf::Vector2f Entity::GetScale()
{
	return _transform.getScale();
}

sf::Transformable Entity::GetTransform()
{
	return _transform;
}

void Entity::SetState(std::string state)
{
	_state = state;
	_animations.SetCurrentState(state);
}

void Entity::SetAnimations(sf::AnimationContainer container)
{
	_animations = container;
	_animations.SetCurrentState(_state);
}

void Entity::SetHealth(float health)
{
	_health = health;
}

void Entity::SetSpeed(float speed)
{
	_speed = speed;
}

void Entity::SetVisibility(bool visibility)
{
	_isVisible = visibility;
}

void Entity::SetPosition(float x, float y)
{
	_animations.ApplySetPosition(x, y);
	_transform.setPosition(x, y);
	SetHitboxPosition(x, y);
}

void Entity::SetPosition(const sf::Vector2f& pos)
{
	_animations.ApplySetPosition(pos);
	_transform.setPosition(pos);
	SetHitboxPosition(pos);
}

void Entity::SetRotation(float angle)
{
	_animations.ApplySetRotation(angle);
	_transform.setRotation(angle);
}

void Entity::SetOrigin(const sf::Vector2f& origin)
{
	_animations.ApplySetOrigin(origin);
	_transform.setOrigin(origin);
}

void Entity::SetOrigin(float x, float y)
{
	_animations.ApplySetOrigin(x, y);
	_transform.setOrigin(x, y);
}

void Entity::SetScale(const sf::Vector2f& factors)
{
	_animations.ApplySetScale(factors);
	_transform.setScale(factors);
	SetHitboxScale(factors);
}

void Entity::SetScale(float factorX, float factorY)
{
	_animations.ApplySetScale(factorX, factorY);
	_transform.setScale(factorX, factorY);
	SetHitboxScale(sf::Vector2f(factorX, factorY));
}

void Entity::SetTransform(const sf::Transformable& trans)
{
	_transform = trans;
	_animations.ApplySetPosition(trans.getPosition());
	_animations.ApplySetRotation(trans.getRotation());
	_animations.ApplySetOrigin(trans.getOrigin());
	_animations.ApplySetScale(trans.getScale());
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(_isVisible)
		target.draw(_animations);
	Collision::draw(target, states);
}
