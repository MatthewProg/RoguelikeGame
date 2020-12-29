#include "Player.h"

Player::Player()
{
	_logger = Logger::GetInstance();
	_playerState = "";
	_playerPosition = sf::Vector2f(0, 0);
	_playerLives = 3;
	_playerSpeed = 1.0;
	_attackCooldown = 1;
	_cooldownCounter = 0;
	_playerHitboxOffset = sf::FloatRect(0, 0, 16, 16);
	_hitboxRectangle.setFillColor(sf::Color(64, 255, 64, 64));
	_hitboxRectangle.setOutlineThickness(-1);
	_hitboxRectangle.setOutlineColor(sf::Color(64, 255, 64, 160));
	_hitboxRectangle.setPosition(_playerHitboxOffset.left, _playerHitboxOffset.top);
	_hitboxRectangle.setSize(sf::Vector2f(_playerHitboxOffset.width, _playerHitboxOffset.height));
	_showHitbox = false;
}

Player::~Player()
{
}

void Player::Tick(bool tick)
{
	_playerAnimation.Tick(tick);
	if (tick && _cooldownCounter < _attackCooldown)
		_cooldownCounter++;
}

sf::AnimationContainer* Player::GetPlayerAnimations()
{
	return &_playerAnimation;
}

sf::Vector2f Player::GetPlayerPosition()
{
	return _playerPosition;
}

std::string Player::GetPlayerState()
{
	return _playerState;
}

short Player::GetPlayerLives()
{
	return _playerLives;
}

float Player::GetPlayerSpeed()
{
	return _playerSpeed;
}

unsigned short Player::GetAttackCooldown()
{
	return _attackCooldown;
}

sf::Color Player::GetHitboxColor()
{
	return _hitboxRectangle.getFillColor();
}

float Player::GetHitboxOutlineThickness()
{
	return _hitboxRectangle.getOutlineThickness();
}

sf::Color Player::GetHitboxOutlineColor()
{
	return _hitboxRectangle.getOutlineColor();
}

bool Player::GetHitboxVisibility()
{
	return _showHitbox;
}

sf::FloatRect Player::GetNextHitboxPosition(float deltaTime)
{
	float moveX = 0;
	float moveY = 0;
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		moveX += (2 * deltaTime * _playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		moveX -= (2 * deltaTime * _playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveY += (2 * deltaTime * _playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveY -= (2 * deltaTime * _playerSpeed);

	auto rect = _hitboxRectangle.getGlobalBounds();
	return sf::FloatRect(rect.left + moveX, rect.top + moveY, rect.width, rect.height);
}

bool Player::CanAttack()
{
	return !(_cooldownCounter < _attackCooldown);
}

void Player::ResetCooldown()
{
	_cooldownCounter = 0;
}

void Player::UpdateMovement(float delta, MapLayerModel<uint8_t>* tiles, uint8_t blockId)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetPlayerState("move");
		auto nextPos = GetNextHitboxPosition(delta);
		auto currPos = GetCollisionBox();
		auto hitbox = GetCollisionBoxOffset();
		if (currPos.left > nextPos.left)
			_playerAnimation.ApplySetHorizontalFlip(true);
		else if(currPos.left < nextPos.left)
			_playerAnimation.ApplySetHorizontalFlip(false);

		auto pos = CollisionHelper::GetLimitPosition(currPos, nextPos, tiles, blockId);
		SetPlayerPosition(sf::Vector2f(pos.x - hitbox.left, pos.y - hitbox.top));
	}
	else
		SetPlayerState("idle");
}

void Player::SetPlayerPosition(const sf::Vector2f& position)
{
	_playerPosition = position;
	_hitboxRectangle.setPosition(sf::Vector2f(position.x + _playerHitboxOffset.left, position.y + _playerHitboxOffset.top));
	_playerAnimation.ApplySetPosition(position);
}

void Player::SetPlayerPosition(float x, float y)
{
	_playerPosition = sf::Vector2f(x, y);
	_hitboxRectangle.setPosition(x + _playerHitboxOffset.left, y + _playerHitboxOffset.top);
	_playerAnimation.ApplySetPosition(x, y);
}

void Player::SetPlayerState(std::string state)
{
	_playerState = state;
	_playerAnimation.SetCurrentState(state);
}

void Player::SetPlayerAnimations(sf::AnimationContainer container)
{
	_playerAnimation = container;
}

void Player::SetPlayerLives(short lives)
{
	_playerLives = lives;
}

void Player::SetPlayerSpeed(float speed)
{
	_playerSpeed = speed;
}

void Player::SetAttackCooldown(unsigned short ticksCooldown)
{
	_attackCooldown = ticksCooldown;
}

void Player::SetHitboxColor(const sf::Color& color)
{
	_hitboxRectangle.setFillColor(color);
}

void Player::SetHitboxOutlineThickness(float thickness)
{
	_hitboxRectangle.setOutlineThickness(thickness);
}

void Player::SetHitboxOutlineColor(const sf::Color& color)
{
	_hitboxRectangle.setOutlineColor(color);
}

void Player::SetHitboxVisibility(bool visible)
{
	_showHitbox = visible;
}

void Player::MoveBy(float x, float y, float deltaTime)
{
	SetPlayerPosition(_playerPosition.x + (x * deltaTime * _playerSpeed), _playerPosition.y + (y * deltaTime * _playerSpeed));
}

void Player::ToggleHitboxVisibility()
{
	std::string status = (!GetHitboxVisibility()) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show player hitbox: " + status);
	SetHitboxVisibility(!GetHitboxVisibility());
}

sf::FloatRect Player::GetCollisionBox()
{
	return _hitboxRectangle.getGlobalBounds();
}

sf::RectangleShape Player::GetHitboxRect()
{
	return _hitboxRectangle;
}

sf::FloatRect Player::GetCollisionBoxOffset()
{
	return _playerHitboxOffset;
}

void Player::SetCollisionBoxOffset(sf::FloatRect rect)
{
	_playerHitboxOffset = rect;

	auto pos = GetPlayerPosition();
	_hitboxRectangle.setPosition(rect.left + pos.x, rect.top + pos.y);
	_hitboxRectangle.setSize(sf::Vector2f(rect.width, rect.height));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_playerAnimation);
	if(_showHitbox)
		target.draw(_hitboxRectangle);
}
