#pragma once

#include "SFML/Graphics/Rect.hpp"

class ViewHelper
{
public:
	static sf::Vector2f GetRectCenter(const sf::FloatRect& rect);
};

