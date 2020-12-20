#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "Utilities.h"
#include "Logger.h"

class TilesHelper
{
private:
public:
	static sf::Texture GetStandardTile(sf::Image img, int width, int height, int id);
	static sf::IntRect GetTileRect(sf::Vector2u imgSize, int width, int height, int id);
	static sf::Texture GetCustomTile(sf::Image img, int left, int top, int width, int height);
};

