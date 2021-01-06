#pragma once

#include "MapLayerModel.h"
#include "MathHelper.h"

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Glsl.hpp"

class CollisionHelper
{
public:
	static bool CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second);
	static bool CheckCircleCollision(const sf::Vector2f point, const sf::Vector2f center, float radius, float arc, float angle);
	static bool CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<bool> *tiles);

	static sf::Vector2f GetTileLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<bool>* tiles);
	static sf::Glsl::Ivec4 GetPosOnTiles(const sf::FloatRect pos, const MapLayerModel<bool>* tiles);
	static std::vector<sf::Vector2f> GetRectPoints(const sf::FloatRect rect);
};