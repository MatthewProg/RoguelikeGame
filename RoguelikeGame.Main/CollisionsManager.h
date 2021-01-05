#pragma once

#include "CollisionHelper.h"
#include "MapLayerModel.h"

class CollisionsManager
{
private:
	std::vector<MapLayerModel<bool>> _maps;
	MapLayerModel<bool> _sumMap;
public:
	CollisionsManager();
	~CollisionsManager();

	template<typename T>
	void AddMap(MapLayerModel<T> map, T block);
	void GenerateCommonMap();

	bool CheckCollision(const sf::FloatRect& rect);

	sf::Vector2f GetLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos);

	std::vector<MapLayerModel<bool>>* GetStoredMaps();
	MapLayerModel<bool>* GetCommonMap();
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
