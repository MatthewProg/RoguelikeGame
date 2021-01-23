#include "Collision.h"

sf::Collision::Collision()
{
	_showHitbox = false;
	_hitboxRecangle.setFillColor(sf::Color(64, 255, 64, 64));
	_hitboxRecangle.setOutlineThickness(-.5F);
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
	auto out = _hitboxOffset;
	auto scale = _hitboxRecangle.getScale();
	out.top *= scale.y;
	out.height *= scale.y;
	out.left *= scale.x;
	out.width *= scale.x;
	return out;
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
	auto scale = _hitboxRecangle.getScale();

	_hitboxRecangle.setPosition(pos.x - (_hitboxOffset.left * scale.x) + (rect.left * scale.x), pos.y - (_hitboxOffset.top * scale.y) + (rect.top * scale.y));
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
	auto scale = _hitboxRecangle.getScale();
	_hitboxRecangle.setPosition(pos.x + (_hitboxOffset.left * scale.x), pos.y + (_hitboxOffset.top * scale.y));
}

void sf::Collision::SetHitboxPosition(float x, float y)
{
	auto scale = _hitboxRecangle.getScale();
	_hitboxRecangle.setPosition(x + (_hitboxOffset.left * scale.x), y + (_hitboxOffset.top * scale.y));
}

void sf::Collision::SetHitboxScale(const sf::Vector2f& scale)
{
	_hitboxRecangle.setScale(scale);

	auto acc = _hitboxRecangle.getPosition();
	_hitboxRecangle.setPosition(acc.x - _hitboxOffset.left + (_hitboxOffset.left * scale.x), acc.y - _hitboxOffset.top + (_hitboxOffset.top * scale.y));
}

sf::Vector2f sf::Collision::GetHitboxScale()
{
	return _hitboxRecangle.getScale();
}

void sf::Collision::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if (_showHitbox)
		target.draw(_hitboxRecangle);
}
