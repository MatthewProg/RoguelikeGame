#include "Player.h"

Player::Player() : Entity()
{
	//Collision changes
	SetHitboxColor(sf::Color(64, 255, 64, 64));
	SetHitboxOutlineColor(sf::Color(64, 255, 64, 160));
	SetHitboxOutlineThickness(-1.F);
	SetHitboxVisibility(false);

	//Entity changes
	SetHealth(3.F);
	SetSpeed(1.F);
	SetVisibility(true);

	//Player
	_logger = Logger::GetInstance();
	_weapon = nullptr;
}

Player::~Player()
{
	if (_weapon != nullptr)
		delete _weapon;
}

void Player::Update(bool tick, float delta)
{
	UpdateEntity(tick);
	_weapon->Update(tick, delta);
	_weapon->setPosition(ViewHelper::GetRectCenter(GetCollisionBox()));
}

sf::FloatRect Player::GetNextHitboxPosition(float deltaTime)
{
	float moveX = 0;
	float moveY = 0;
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		moveX += (2 * deltaTime * GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		moveX -= (2 * deltaTime * GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		moveY += (2 * deltaTime * GetSpeed());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		moveY -= (2 * deltaTime * GetSpeed());

	auto rect = GetCollisionBox();
	return sf::FloatRect(rect.left + moveX, rect.top + moveY, rect.width, rect.height);
}

void Player::UpdateMovement(float delta, MapLayerModel<uint8_t>* tiles, uint8_t blockId)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetState("move");
		auto nextPos = GetNextHitboxPosition(delta);
		auto currPos = GetCollisionBox();
		auto hitbox = GetCollisionBoxOffset();
		if (currPos.left > nextPos.left)
			GetAnimations()->ApplySetHorizontalFlip(true);
		else if(currPos.left < nextPos.left)
			GetAnimations()->ApplySetHorizontalFlip(false);

		auto pos = CollisionHelper::GetLimitPosition(currPos, nextPos, tiles, blockId);
		SetPosition(sf::Vector2f(pos.x - hitbox.left, pos.y - hitbox.top));
	}
	else
		SetState("idle");
}

void Player::MoveBy(float x, float y, float deltaTime)
{
	auto pos = GetPosition();
	SetPosition(pos.x + (x * deltaTime * GetSpeed()), pos.y + (y * deltaTime * GetSpeed()));
}

void Player::ToggleHitboxVisibility()
{
	std::string status = (!GetHitboxVisibility()) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show player hitbox: " + status);
	SetHitboxVisibility(!GetHitboxVisibility());
}

void Player::ToggleWeaponHitboxVisibility()
{
	if (_weapon != nullptr)
	{
		bool rev = _weapon->GetHitboxVisibility();
		std::string status = (!rev) ? "true" : "false";
		_logger->Log(Logger::LogType::INFO, "Show current weapon rays: " + status);
		_weapon->SetHitboxVisibility(!rev);
	}
}

Weapon* Player::GetWeapon()
{
	return _weapon;
}

void Player::SetWeapon(Weapon* weapon)
{
	if (_weapon != nullptr)
		delete _weapon;

	_weapon = weapon;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	_weapon->draw(target, states);
}
