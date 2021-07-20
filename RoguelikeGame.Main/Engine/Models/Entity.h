#pragma once

#include "../Utilities/AnimationContainer.h"
#include "../Utilities/Collision.h"
#include "../Managers/SoundsManager.h"

class Entity : public sf::Collision
{
private:
	std::string _state;
	sf::AnimationContainer _animations;

	float _health;
	float _speed;
	float _step;

	sf::Transformable _transform;

	sf::FloatRect _viewRect;

	bool _isVisible;

	sf::Color _dmgColor;
	unsigned short _dmgColorTick;
	unsigned short _dmgColorCounter;
protected:
	SoundsManager* _sounds;
	std::vector<std::string> _takingDmgSounds;

	void UpdateEntity(bool tick);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Entity();
	Entity(Entity& other);
	~Entity() override = default;

	void TakeDmg(float dmg);
	bool IsDead() const;

	sf::AnimationContainer* GetAnimations();
	std::string GetState();
	float GetHealth() const;
	float GetSpeed() const;
	float GetStep() const;
	bool GetVisibility() const; 
	const sf::Vector2f& GetPosition() const;
	float GetRotation() const;
	const sf::Vector2f& GetOrigin() const;
	const sf::Vector2f& GetScale() const;
	const sf::Transformable& GetTransform() const;
	const sf::Color& GetTakingDmgColor() const;
	unsigned short GetTakingDmgColorTicks() const;
	sf::FloatRect GetView() const;

	void SetState(const std::string& state);
	void SetAnimations(const sf::AnimationContainer& container);
	void SetHealth(float health);
	void SetSpeed(float speed);
	void SetStep(float step);
	void SetVisibility(bool visibility);
	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& pos);
	void SetRotation(float angle);
	void SetOrigin(const sf::Vector2f& origin);
	void SetOrigin(float x, float y);
	void SetScale(const sf::Vector2f& factors);
	void SetScale(float factorX, float factorY);
	void SetTransform(const sf::Transformable& trans);
	void SetTakingDmgColor(const sf::Color& color);
	void SetTakingDmgColorTicks(unsigned short ticks);
	void SetView(const sf::FloatRect& rect);

	void SetSoundsManager(SoundsManager* manager);
	void AddTakingDmgSound(const std::string& sound);
	void ClearTakingDmgSounds();
	const std::vector<std::string>* GetTakingDmgSounds() const;
};

