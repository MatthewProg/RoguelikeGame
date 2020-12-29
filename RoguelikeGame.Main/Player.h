#pragma once

#include "AnimationContainer.h"
#include "CollisionHelper.h"
#include "MapLayerModel.h"
#include "Collision.h"

#include "Logger.h"

class Player : public sf::Drawable, public sf::Collision
{
private:
	sf::AnimationContainer _playerAnimation;
	std::string _playerState;

	short _playerLives;
	float _playerSpeed;
	unsigned short _attackCooldown;
	unsigned int _cooldownCounter;

	bool _showHitbox;

	sf::FloatRect _playerHitboxOffset;
	sf::RectangleShape _hitboxRectangle;

	sf::Vector2f _playerPosition;

	Logger* _logger;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	
	Player();
	~Player();

	//Player
	void Tick(bool tick);
	bool CanAttack();
	void ResetCooldown();
	void UpdateMovement(float delta, MapLayerModel<uint8_t>* tiles, uint8_t blockId);

	//Player getters
	sf::AnimationContainer* GetPlayerAnimations();
	sf::Vector2f GetPlayerPosition();
	std::string GetPlayerState();
	short GetPlayerLives();
	float GetPlayerSpeed();
	unsigned short GetAttackCooldown();
	sf::Color GetHitboxColor();
	float GetHitboxOutlineThickness();
	sf::Color GetHitboxOutlineColor();
	bool GetHitboxVisibility();
	sf::FloatRect GetNextHitboxPosition(float deltaTime);

	//Player setters
	void SetPlayerPosition(const sf::Vector2f& position);
	void SetPlayerPosition(float x, float y);
	void SetPlayerState(std::string state);
	void SetPlayerAnimations(sf::AnimationContainer container);
	void SetPlayerLives(short lives);
	void SetPlayerSpeed(float speed);
	void SetAttackCooldown(unsigned short ticksCooldown);
	void SetHitboxColor(const sf::Color& color);
	void SetHitboxOutlineThickness(float thickness);
	void SetHitboxOutlineColor(const sf::Color& color);
	void SetHitboxVisibility(bool visible);
	void MoveBy(float x, float y, float deltaTime);

	void ToggleHitboxVisibility();

	// Inherited via Collision
	virtual sf::FloatRect GetCollisionBox() override;
	virtual sf::RectangleShape GetHitboxRect() override;
	virtual sf::FloatRect GetCollisionBoxOffset() override;
	virtual void SetCollisionBoxOffset(sf::FloatRect rect) override;
};

