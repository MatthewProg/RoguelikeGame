#pragma once

#include "MapLayerModel.h"

#include "SFML/Graphics/Rect.hpp"

class CollisionHelper
{
public:
	static bool CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second);
	static bool CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<uint8_t> *tiles, const uint8_t blockId);

	static sf::Vector2f GetLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<uint8_t>* tiles, const uint8_t blockId);
};