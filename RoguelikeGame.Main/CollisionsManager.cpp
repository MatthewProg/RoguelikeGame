#include "CollisionsManager.h"

CollisionsManager::CollisionsManager()
{
	_maps.clear();
}

CollisionsManager::~CollisionsManager()
{
}

void CollisionsManager::GenerateCommonMap()
{
	if (_maps.size() <= 0) return;

	_sumMap.offsetX = _maps[0].offsetX;
	_sumMap.offsetY = _maps[0].offsetY;
	_sumMap.tileHeight = _maps[0].tileHeight;
	_sumMap.tileWidth = _maps[0].tileWidth;
	_sumMap.opacity = _maps[0].opacity;
	_sumMap.visible = _maps[0].visible;
	_sumMap.tilesName = _maps[0].tilesName;

	auto compareMapWidth =  [](const MapLayerModel<bool>& a, const MapLayerModel<bool>& b) { return (a.width < b.width); };
	auto compareMapHeight = [](const MapLayerModel<bool>& a, const MapLayerModel<bool>& b) { return (a.height < b.height); };

	_sumMap.height = std::max_element(_maps.begin(), _maps.end(), compareMapHeight)->height;
	_sumMap.width = std::max_element(_maps.begin(), _maps.end(), compareMapWidth)->width;

	_sumMap.data.resize(_sumMap.height * _sumMap.width, false);
	for (auto it = _maps.begin(); it != _maps.end(); it++)
		for (size_t i = 0; i < it->data.size(); i++)
			if (it->data[i])
				_sumMap.data[(i / it->width) * _sumMap.width + (i % it->width)] = true;
}

bool CollisionsManager::CheckCollision(const sf::FloatRect& rect)
{
	return CollisionHelper::CheckTileCollision(rect, &_sumMap);
}

sf::Vector2f CollisionsManager::GetLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos)
{
	return CollisionHelper::GetTileLimitPosition(startPos, endPos, &_sumMap);
}

std::vector<MapLayerModel<bool>>* CollisionsManager::GetStoredMaps()
{
	return &_maps;
}

MapLayerModel<bool>* CollisionsManager::GetCommonMap()
{
	return &_sumMap;
}
