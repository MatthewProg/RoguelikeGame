#pragma once

#include "SFML/Graphics/Rect.hpp"

class ViewHelper
{
public:
	static sf::Vector2f GetRectCenter(const sf::FloatRect& rect);
	static sf::FloatRect GetScaled(const sf::FloatRect& scale, const sf::FloatRect& element, const sf::FloatRect& relativeTo);
};

