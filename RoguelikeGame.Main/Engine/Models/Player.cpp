#include "Player.h"

Player::Player() : Entity()
{
	//Collision changes
	SetHitboxColor(sf::Color(64, 255, 64, 64));
	SetHitboxOutlineColor(sf::Color(64, 255, 64, 160));
	SetHitboxOutlineThickness(-.5F);
	SetHitboxVisibility(false);

	//Entity changes
	SetHealth(3.F);
	SetSpeed(1.F);
	SetVisibility(true);
	SetView(sf::FloatRect(-150, -94, 300, 188));

	//Player
	_logger = Logger::GetInstance();
	_weapon = nullptr;
}

Player::Player(Player& other) : Entity(other)
{
	_weapon = other._weapon;
	_logger = other._logger;
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

void Player::ToggleHitboxVisibility()
{
	std::string status = (!GetHitboxVisibility()) ? "true" : "false";
	_logger->Log(Logger::LogType::DEBUG, "Show player hitbox: " + status);
	SetHitboxVisibility(!GetHitboxVisibility());
}

void Player::ToggleWeaponHitboxVisibility()
{
	if (_weapon != nullptr)
	{
		bool rev = _weapon->GetHitboxVisibility();
		std::string status = (!rev) ? "true" : "false";
		_logger->Log(Logger::LogType::DEBUG, "Show current weapon rays: " + status);
		_weapon->SetHitboxVisibility(!rev);
	}
}

void Player::ToggleRaycastVisibility()
{
	if (_weapon != nullptr)
	{
		std::string status = (!_weapon->GetRaycastVisibility()) ? "true" : "false";
		_logger->Log(Logger::LogType::DEBUG, "Show player weapon raycast: " + status);
		_weapon->SetRaycastVisibility(!_weapon->GetRaycastVisibility());
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

void Player::SetWeaponUnsafe(Weapon* weapon)
{
	_weapon = weapon;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	if(_weapon != nullptr)
		_weapon->draw(target, states);
}
