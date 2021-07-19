#include "Entity.h"

void Entity::UpdateEntity(bool tick)
{
	_animations.Tick(tick);

	if (tick)
	{
		if (_dmgColorCounter == _dmgColorTick)
		{
			_animations.ApplySetColor(sf::Color::White);
			if (_health <= 0) _isVisible = false;
		}
		else
			_dmgColorCounter++;
	}
}

Entity::Entity() : sf::Collision()
{
	_state = "";
	_health = 3.F;
	_speed = 1.F;
	_step = 2.F;
	_isVisible = true;
	_dmgColor = sf::Color(255, 32, 32, 255);
	_dmgColorTick = 15;
	_dmgColorCounter = 0;
	_viewRect = sf::FloatRect(-150,-94, 300, 188);
	_sounds = nullptr;
	_takingDmgSounds.clear();
}

Entity::Entity(Entity& other) : sf::Collision(other)
{
	_animations = other._animations;
	_dmgColor = other._dmgColor;
	_dmgColorCounter = other._dmgColorCounter;
	_dmgColorTick = other._dmgColorTick;
	_health = other._health;
	_isVisible = other._isVisible;
	_speed = other._speed;
	_state = other._state;
	_step = other._step;
	_transform = other._transform;
	_viewRect = other._viewRect;
	_sounds = other._sounds;
	_takingDmgSounds = other._takingDmgSounds;
}

void Entity::TakeDmg(float dmg)
{
	_health -= dmg;
	_animations.ApplySetColor(_dmgColor);
	_dmgColorCounter = 0;
	
	if (_takingDmgSounds.size() > 0)
	{
		auto rng = rand() % _takingDmgSounds.size();
		_sounds->PlaySoundIndependent(_takingDmgSounds[rng]);
	}
}

bool Entity::IsDead() const
{
	return (_isVisible == false && _health <= 0);
}

sf::AnimationContainer* Entity::GetAnimations()
{
	return &_animations;
}

std::string Entity::GetState()
{
	_state = _animations.GetCurrentState();
	return _state;
}

float Entity::GetHealth() const
{
	return _health;
}

float Entity::GetSpeed() const
{
	return _speed;
}

float Entity::GetStep() const
{
	return _step;
}

bool Entity::GetVisibility() const
{
	return _isVisible;
}

const sf::Vector2f& Entity::GetPosition() const
{
	return _transform.getPosition();
}

float Entity::GetRotation() const
{
	return _transform.getRotation();
}

const sf::Vector2f& Entity::GetOrigin() const
{
	return _transform.getOrigin();
}

const sf::Vector2f& Entity::GetScale() const
{
	return _transform.getScale();
}

const sf::Transformable& Entity::GetTransform() const
{
	return _transform;
}

const sf::Color& Entity::GetTakingDmgColor() const
{
	return _dmgColor;
}

unsigned short Entity::GetTakingDmgColorTicks() const
{
	return _dmgColorTick;
}

sf::FloatRect Entity::GetView() const
{
	auto output = _viewRect;
	auto &pos = GetPosition();

	output.left += pos.x;
	output.top  += pos.y;

	return output;
}

void Entity::SetState(const std::string& state)
{
	_state = state;
	_animations.SetCurrentState(state);
}

void Entity::SetAnimations(const sf::AnimationContainer& container)
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

void Entity::SetStep(float step)
{
	_step = step;
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

void Entity::SetTakingDmgColor(const sf::Color& color)
{
	_dmgColor = color;
}

void Entity::SetTakingDmgColorTicks(unsigned short ticks)
{
	_dmgColorTick = ticks;
}

void Entity::SetView(const sf::FloatRect& rect)
{
	_viewRect = rect;
}

void Entity::SetSoundsManager(SoundsManager* manager)
{ 
	_sounds = manager;
}

void Entity::AddTakingDmgSound(const std::string& sound)
{
	_takingDmgSounds.push_back(sound);
}

void Entity::ClearTakingDmgSounds()
{
	_takingDmgSounds.clear();
}

const std::vector<std::string>* Entity::GetTakingDmgSounds() const
{
	return &_takingDmgSounds;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(_isVisible)
		target.draw(_animations);
	Collision::draw(target, states);
}
