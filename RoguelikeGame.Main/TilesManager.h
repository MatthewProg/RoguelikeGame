#pragma once

#include <SFML/Graphics.hpp>
#include "Logger.h"

class TilesManager
{
private:
public:
	static sf::Texture GetStandardTile(sf::Image img, int width, int height, int id);
	static sf::Texture GetCustomTile(sf::Image img, int left, int top, int width, int height);
};

