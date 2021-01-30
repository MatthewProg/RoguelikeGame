#pragma once

#include "../Models/MapLayerModel.h"
#include "../Helpers/MathHelper.h"

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Glsl.hpp"

class CollisionHelper
{
public:
	static bool CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second);
	static bool CheckCircleCollision(const sf::Vector2f point, const sf::Vector2f center, float radius, float arc, float angle);
	static bool CheckCircleCollision(const sf::Vector2f point, const sf::Vector2f center, float radius);
	static bool CheckCirclesIntersect(const sf::Vector2f center1, float radius1, const sf::Vector2f center2, float radius2);
	static bool CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<bool> *tiles);
	static bool CheckTileCollision(const sf::Vector2f center, float radius, const MapLayerModel<bool>* tiles);

	static sf::Vector2f GetTileLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<bool>* tiles);
	static sf::Vector2f GetTileLimitPosition(const sf::Vector2f startPos, const sf::Vector2f endPos, float radius, const MapLayerModel<bool>* tiles);
	static sf::Vector2f GetRectLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const sf::FloatRect block);
	static sf::Glsl::Ivec4 GetPosOnTiles(const sf::FloatRect pos, const MapLayerModel<bool>* tiles);
	static sf::Vector2i GetPosOnTiles(const sf::Vector2f pos, const MapLayerModel<bool>* tiles);
	static std::vector<sf::Vector2f> GetRectPoints(const sf::FloatRect rect);
};