#pragma once

#include "AnimationContainer.h"
#include "Collision.h"

class Entity : public sf::Collision
{
private:
	std::string _state;
	sf::AnimationContainer _animations;

	float _health;
	float _speed;

	sf::Transformable _transform;

	bool _isVisible;
protected:
	void UpdateEntity(bool tick);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Entity();
	~Entity();

	sf::AnimationContainer* GetAnimations();
	std::string GetState();
	float GetHealth();
	float GetSpeed();
	bool GetVisibility(); 
	sf::Vector2f GetPosition();
	float GetRotation();
	sf::Vector2f GetOrigin();
	sf::Vector2f GetScale();
	sf::Transformable GetTransform();

	void SetState(std::string state);
	void SetAnimations(sf::AnimationContainer container);
	void SetHealth(float health);
	void SetSpeed(float speed);
	void SetVisibility(bool visibility);
	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& pos);
	void SetRotation(float angle);
	void SetOrigin(const sf::Vector2f& origin);
	void SetOrigin(float x, float y);
	void SetScale(const sf::Vector2f& factors);
	void SetScale(float factorX, float factorY);
	void SetTransform(const sf::Transformable& trans);
};

