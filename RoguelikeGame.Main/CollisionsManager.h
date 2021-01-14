#pragma once

#include "Logger.h"

#include "CollisionHelper.h"
#include "MapLayerModel.h"
#include <tuple>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Drawable.hpp"

class CollisionsManager : public sf::Drawable
{
private:
	Logger* _logger;

	std::vector<MapLayerModel<bool>> _maps;
	MapLayerModel<bool> _sumMap;

	std::vector<std::tuple<sf::Vector2f, sf::Vector2f>> _edges;
	sf::VertexArray _edgesLines;

	sf::Color _linesColor;

	bool _showCollisionLines;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	CollisionsManager();
	~CollisionsManager();

	template<typename T>
	void AddMap(MapLayerModel<T> map, T block);

	void GenerateCommonMap();
	void CovertTilesIntoEdges();

	//Manager setters
	void SetCollisionLinesColor(sf::Color color);
	void SetCollisionLinesVisibility(bool visible);
	void ToggleCollisionLinesVisibility();

	//Manager getters
	sf::Color GetCollisionLinesColor();
	bool GetCollisionLinesVisibility();

	//Collision methods
	bool CheckCollision(const sf::FloatRect& rect);
	sf::Vector2f GetLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos);
	sf::Vector2f GetRayHitpoint(const sf::Vector2f& center, float angle, float raycastRange);
	bool RaycastHitsPoint(const sf::Vector2f& startPos, const sf::Vector2f& endPos, float* distanceToHitpoint);

	//Var access methods
	std::vector<MapLayerModel<bool>>* GetStoredMaps();
	MapLayerModel<bool>* GetCommonMap();
	std::vector<std::tuple<sf::Vector2f, sf::Vector2f>> GetEdges();
};

	template<typename T>
	inline void CollisionsManager::AddMap(MapLayerModel<T> map, T block)
	{
		MapLayerModel<bool> blocked;
		blocked.id = map.id;
		blocked.height = map.height;
		blocked.width = map.width;
		blocked.opacity = map.opacity;
		blocked.offsetX = map.offsetX;
		blocked.offsetY = map.offsetY;
		blocked.tileHeight = map.tileHeight;
		blocked.tileWidth = map.tileWidth;
		blocked.tilesName = map.tilesName;
		blocked.visible = map.visible;
		blocked.data.resize(map.data.size(), false);

		for (size_t i = 0; i < map.data.size(); i++)
			if (map.data[i] == block)
				blocked.data[i] = true;

		_maps.push_back(blocked);
};
