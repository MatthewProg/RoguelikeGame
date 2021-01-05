#pragma once

#include "MapLayerModel.h"

#include "SFML/Graphics/Rect.hpp"

class CollisionHelper
{
public:
	static bool CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second);
	static bool CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<bool> *tiles);

	static sf::Vector2f GetTileLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<bool>* tiles);
};