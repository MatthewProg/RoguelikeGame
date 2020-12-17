#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace sf
{
	class Collision
	{
	public:
		virtual sf::FloatRect GetCollisionBox() = 0;
		virtual sf::FloatRect GetCollisionBoxOffset() = 0;
		virtual sf::RectangleShape GetHitboxRect() = 0;
		virtual void SetCollisionBoxOffset(sf::FloatRect rect) = 0;
	};
}

