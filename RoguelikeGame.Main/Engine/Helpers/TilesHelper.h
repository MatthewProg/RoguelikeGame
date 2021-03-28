#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "../Utilities/Utilities.h"
#include "../Core/Logger.h"

class TilesHelper
{
private:
public:
	static sf::Texture GetStandardTile(const sf::Image& img, unsigned int width, unsigned int height, int id);
	static sf::IntRect GetTileRect(const sf::Vector2u& imgSize, unsigned int width, unsigned int height, int id);
	static sf::Texture GetCustomTile(const sf::Image& img, unsigned int left, unsigned int top, unsigned int width, unsigned int height);
};

