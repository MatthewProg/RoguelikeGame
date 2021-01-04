#include "Collision.h"

sf::Collision::Collision()
{
	_showHitbox = false;
	_hitboxRecangle.setFillColor(sf::Color(64, 255, 64, 64));
	_hitboxRecangle.setOutlineThickness(-1);
	_hitboxRecangle.setOutlineColor(sf::Color(64, 255, 64, 160));
	_hitboxRecangle.setSize(sf::Vector2f(16, 16));
	_hitboxRecangle.setPosition(0, 0);
	_hitboxOffset = sf::FloatRect(0, 0, 16, 16);
}

sf::Collision::~Collision()
{
}

sf::FloatRect sf::Collision::GetCollisionBox()
{
	return _hitboxRecangle.getGlobalBounds();
}

sf::FloatRect sf::Collision::GetCollisionBoxOffset()
{
	return _hitboxOffset;
}

sf::RectangleShape sf::Collision::GetHitboxRectangle()
{
	return _hitboxRecangle;
}

sf::Color sf::Collision::GetHitboxColor()
{
	return _hitboxRecangle.getFillColor();
}

sf::Color sf::Collision::GetHitboxOutlineColor()
{
	return _hitboxRecangle.getOutlineColor();
}

float sf::Collision::GetHitboxOutlineThickness()
{
	return _hitboxRecangle.getOutlineThickness();
}

bool sf::Collision::GetHitboxVisibility()
{
	return _showHitbox;
}

sf::Vector2f sf::Collision::GetHitboxPosition()
{
	return _hitboxRecangle.getPosition();
}

void sf::Collision::SetCollisionBoxOffset(sf::FloatRect rect)
{
	auto pos = _hitboxRecangle.getPosition();

	_hitboxRecangle.setPosition(rect.left - _hitboxOffset.left + pos.x, rect.top - _hitboxOffset.top + pos.y);
	_hitboxRecangle.setSize(sf::Vector2f(rect.width, rect.height));

	_hitboxOffset = rect;
}

void sf::Collision::SetHitboxColor(const sf::Color& color)
{
	_hitboxRecangle.setFillColor(color);
}

void sf::Collision::SetHitboxOutlineColor(const sf::Color& color)
{
	_hitboxRecangle.setOutlineColor(color);
}

void sf::Collision::SetHitboxOutlineThickness(float thickness)
{
	_hitboxRecangle.setOutlineThickness(thickness);
}

void sf::Collision::SetHitboxVisibility(bool visible)
{
	_showHitbox = visible;
}

void sf::Collision::SetHitboxPosition(sf::Vector2f pos)
{
	_hitboxRecangle.setPosition(pos.x + _hitboxOffset.left, pos.y + _hitboxOffset.top);
}

void sf::Collision::SetHitboxPosition(float x, float y)
{
	_hitboxRecangle.setPosition(x + _hitboxOffset.left, y + _hitboxOffset.top);
}

void sf::Collision::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (_showHitbox)
		target.draw(_hitboxRecangle);
}
