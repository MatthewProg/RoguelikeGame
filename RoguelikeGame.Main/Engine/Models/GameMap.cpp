#include "GameMap.h"

template<typename T>
GameMap<T>::GameMap()
{
	_map.clear();
	_layersIds.clear();
	_pathfingingPoints.clear();
	_actionMap.visible = false;
	_actionMap.opacity = 0.5;
	_actionMap.tilesName = "special";
	_logger = Logger::GetInstance();
	_noTexture = Utilities::GetInstance()->NoTexture16x16();
	_showGrid = false;
	_actionMapGridColor = sf::Color(0, 0, 0, 255);
}

template<typename T>
GameMap<T>::~GameMap()
{
}

template<typename T>
bool GameMap<T>::LoadFromFile(const std::string& path)
{
	std::ifstream input;
	input.open(path, std::ios::in);
	if (!input.is_open() || !input.good())
	{
		_logger->Log(Logger::LogType::ERROR, "Unable to load game map!");
		return false;
	}

	nlohmann::json doc;
	input >> doc;

	input.close();

	_mapSize = sf::Vector2u(doc["width"].get<unsigned int>(),doc["height"].get<unsigned int>());

	//Layers
	for (auto& v : doc["layers"])
	{
		auto id = v["id"].get<unsigned int>();
		_layersIds.push_back(id);

		_map[id] = MapLayerModel<T>();
		_map[id].id = id;
		_map[id].height = v["height"].get<unsigned int>();
		_map[id].width = v["width"].get<unsigned int>();
		_map[id].offsetX = v["x"].get<float>();
		_map[id].offsetY = v["y"].get<float>();
		_map[id].visible = v["visible"].get<bool>();
		_map[id].opacity = v["opacity"].get<float>();
		_map[id].tileHeight = v["tileHeight"].get<unsigned int>();
		_map[id].tileWidth = v["tileWidth"].get<unsigned int>();
		_map[id].tilesName = v["tiles"].get<std::string>();
		for (auto& dat : v["data"])
			_map[id].data.push_back(dat.get<T>());
	}

	//Action map
	_actionMap.height = doc["actionLayer"]["height"].get<unsigned int>();
	_actionMap.width = doc["actionLayer"]["width"].get<unsigned int>();
	_actionMap.offsetX = doc["actionLayer"]["x"].get<float>();
	_actionMap.offsetY = doc["actionLayer"]["y"].get<float>();
	_actionMap.tileHeight = doc["actionLayer"]["tileHeight"].get<unsigned int>();
	_actionMap.tileWidth = doc["actionLayer"]["tileWidth"].get<unsigned int>();
	for (auto& dat : doc["actionLayer"]["data"])
		_actionMap.data.push_back(dat.get<unsigned char>());

	//Pathfind
	for (auto& vec : doc["pathfindPoints"])
	{
		float x = vec["x"].get<float>();
		float y = vec["y"].get<float>();
		_pathfingingPoints.push_back(sf::Vector2f(x, y));
	}
		
	std::sort(_layersIds.begin(), _layersIds.end());

	return true;
}

template<typename T>
void GameMap<T>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture)
{
	_tilesTextures[tilesName] = texture;
}

template<typename T>
bool GameMap<T>::AutoSetTilesTextures(TexturesManager* manager)
{
	bool allOk = true;
	auto mapTiles = GetLayersTilesNames();
	
	auto exist = std::find(mapTiles.begin(), mapTiles.end(), _actionMap.tilesName);
	if (exist == mapTiles.end())
		mapTiles.push_back(_actionMap.tilesName);

	for (size_t tile = 0; tile < mapTiles.size(); tile++)
	{
		if (manager->Exists(mapTiles[tile]) == false)
		{
			_logger->Log(Logger::LogType::ERROR, "Graphics (" + std::to_string(tile + 1) + "/" + std::to_string(mapTiles.size()) + "): ERROR");
			allOk = false;
			continue;
		}
		else
		{
			SetTilesTexture(mapTiles[tile], manager->GetTexture(mapTiles[tile]));
			_logger->Log(Logger::LogType::INFO, "Graphics (" + std::to_string(tile + 1) + "/" + std::to_string(mapTiles.size()) + "): OK");
		}
	}
	return allOk;
}

template<typename T>
const sf::Vector2u& GameMap<T>::GetMapSize() const
{
	return _mapSize;
}

template<typename T>
unsigned int GameMap<T>::GetNoOfLayers() const
{
	return _layersIds.size();
}

template<typename T>
const std::vector<unsigned int>& GameMap<T>::GetLayersIds() const
{
	return _layersIds;
}

template<typename T>
std::vector<std::string> GameMap<T>::GetLayersTilesNames() const
{
	std::map<std::string, bool> all;
	for (auto& v : _map)
		all[v.second.tilesName] = true;
	if(_actionMap.visible == true)
		all[_actionMap.tilesName] = true;

	std::vector<std::string> output;
	for (auto& val : all)
		output.push_back(val.first);
	return output;
}

template<typename T>
bool GameMap<T>::GetLayerVisibility(unsigned int layerId) const
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		return found->second.visible;
	return false;
}

template<typename T>
float GameMap<T>::GetLayerOpacity(unsigned int layerId) const
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		return found->second.opacity;
	return 0.0f;
}

template<typename T>
sf::Vector2u GameMap<T>::GetLayerSize(unsigned int layerId) const
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		return sf::Vector2u(found->second.width, found->second.height);
	return sf::Vector2u(0, 0);
}

template<typename T>
sf::Vector2f GameMap<T>::GetLayerOffset(unsigned int layerId) const
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		return sf::Vector2f(found->second.offsetX, found->second.offsetY);
	return sf::Vector2f(0, 0);
}

template<typename T>
std::string GameMap<T>::GetLayerTilesName(unsigned int layerId) const
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		return found->second.tilesName;
	return std::string();
}

template<typename T>
const MapLayerModel<unsigned char>* GameMap<T>::GetActionMap() const
{
	return &_actionMap;
}

template<typename T>
const std::vector<sf::Vector2f>& GameMap<T>::GetPathfindingPoints() const
{
	return _pathfingingPoints;
}

template<typename T>
const sf::Color& GameMap<T>::GetActionMapGridColor() const
{
	return _actionMapGridColor;
}

template<typename T>
bool GameMap<T>::GetActionMapGridVisibility() const
{
	return _showGrid;
}

template<typename T>
void GameMap<T>::SetLayerVisibility(unsigned int layerId, bool visibility)
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		found->second.visible = visibility;
	return;
}

template<typename T>
void GameMap<T>::SetLayerOpacity(unsigned int layerId, float opacity)
{
	float opa = opacity;
	if (opacity > 1) opa = 1;
	if (opacity < 0) opa = 0;

	auto found = _map.find(layerId);
	if (found != _map.end())
	{
		found->second.opacity = opa;
		SetLayerVertexOpacity(layerId, opa);
	}
	return;
}

template<typename T>
void GameMap<T>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset)
{
	auto found = _map.find(layerId);
	if (found != _map.end())
	{
		found->second.offsetX = offset.x;
		found->second.offsetY = offset.y;
		SetLayerVertexOffset(layerId, offset);
	}
	return;
}

template<typename T>
void GameMap<T>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName)
{
	auto found = _map.find(layerId);
	if (found != _map.end())
		found->second.tilesName = tilesName;
	return;
}

template<typename T>
void GameMap<T>::ApplyLayerVisibility(bool visibility)
{
	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
		iter->second.visible = visibility;
}

template<typename T>
void GameMap<T>::ApplyLayerOpacity(float opacity)
{
	float opa = opacity;
	if (opacity > 1) opa = 1;
	if (opacity < 0) opa = 0;

	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
	{
		iter->second.opacity = opa;
		SetLayerVertexOpacity(iter->first, opa);
	}
}

template<typename T>
void GameMap<T>::ApplyLayerOffset(const sf::Vector2f& offset)
{
	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
	{
		iter->second.offsetX = offset.x;
		iter->second.offsetY = offset.y;
		SetLayerVertexOffset(iter->first, offset);
	}
}

template<typename T>
void GameMap<T>::ApplyLayerTilesName(const std::string& tilesName)
{
	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
		iter->second.tilesName = tilesName;
}

template<typename T>
void GameMap<T>::SetActionMapVisibility(bool visibility)
{
	_actionMap.visible = visibility;
	PrepareActionMapLayer();
}

template<typename T>
void GameMap<T>::SetActionMapOpacity(float opacity)
{
	float opa = opacity;
	if (opacity > 1) opa = 1;
	if (opacity < 0) opa = 0;

	_actionMap.opacity = opa;

	sf::Color color = sf::Color(255, 255, 255, (sf::Uint8)(opacity * 255));
	for (size_t no = 0; no < _actionMapVertices.getVertexCount(); no++)
		_actionMapVertices[no].color = color;
}

template<typename T>
void GameMap<T>::SetActionMapOffset(const sf::Vector2f& offset)
{
	_actionMap.offsetX = offset.x;
	_actionMap.offsetY = offset.y;
	_actionMapTransform.setPosition(offset);
}

template<typename T>
void GameMap<T>::SetActionMapTilesName(const std::string& tilesName)
{
	_actionMap.tilesName = tilesName;
}

template<typename T>
void GameMap<T>::SetActionMapGridColor(const sf::Color& col)
{
	_actionMapGridColor = col;
	for (size_t i = 0; i < _actionMapGrid.getVertexCount(); i++)
		_actionMapGrid[i].color = col;
}

template<typename T>
void GameMap<T>::SetActionMapGridVisibility(bool visible)
{
	_showGrid = visible;
	if (_showGrid) PrepareActionMapGrid();
}

template<typename T>
void GameMap<T>::ToggleGridVisibility()
{
	std::string status = (!_showGrid) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show grid: " + status);
	SetActionMapGridVisibility(!_showGrid);
}

template<typename T>
void GameMap<T>::ToggleActionMapVisibility()
{
	std::string status = (!_actionMap.visible) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show action map: " + status);
	SetActionMapVisibility(!_actionMap.visible);
}

template<typename T>
void GameMap<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto id : _layersIds)
	{
		auto found = _map.find(id);
		if (found == _map.end()) continue;
		if (found->second.visible == false) continue;

		auto texture = _tilesTextures.find(found->second.tilesName);
		if (texture == _tilesTextures.end())
			states.texture = _noTexture;
		else if (texture->second == nullptr)
			states.texture = _noTexture;
		else if (texture->second->getSize() == sf::Vector2u())
			states.texture = _noTexture;
		else
			states.texture = texture->second;

		states.transform = _layerTransform.find(id)->second.getTransform();

		target.draw(_layerVertices.find(id)->second, states);
	}

	//Action map
	if (_actionMap.visible)
	{
		auto texture = _tilesTextures.find(_actionMap.tilesName);
		if (texture == _tilesTextures.end())
			states.texture = _noTexture;
		else if (texture->second->getSize() == sf::Vector2u())
			states.texture = _noTexture;
		else
			states.texture = texture->second;

		states.transform = _actionMapTransform.getTransform();
		target.draw(_actionMapVertices, states);
	}

	//Grid
	if (_showGrid)
	{
		states.transform = _actionMapTransform.getTransform();
		states.texture = nullptr;
		target.draw(_actionMapGrid, states);
	}
}

template<typename T>
void GameMap<T>::PrepareFrame()
{
	if (_layerVertices.size() != _layersIds.size())
		_layerVertices.clear();
	if (_layerTransform.size() != _layersIds.size())
		_layerTransform.clear();

	for (auto id : _layersIds)
	{
		auto vertex = &_layerVertices[id];
		vertex->setPrimitiveType(sf::Quads);

		auto layer = _map.find(id);
		if (layer == _map.end())
		{
			vertex->resize(0);
			continue;
		}
		if (layer->second.visible == false)
		{
			vertex->resize(0);
			continue;
		}

		auto height = layer->second.height;
		auto width = layer->second.width;
		auto offsetX = layer->second.offsetX;
		auto offsetY = layer->second.offsetY;
		auto tileWidth = layer->second.tileWidth;
		auto tileHeight = layer->second.tileHeight;

		auto tilesName = layer->second.tilesName;
		sf::Color opacity = sf::Color(255, 255, 255, (sf::Uint8)(layer->second.opacity * 255));

		sf::Texture* texture = nullptr;
		if (_tilesTextures.find(tilesName) == _tilesTextures.end())
			texture = _noTexture;
		else if (_tilesTextures[tilesName]->getSize() == sf::Vector2u(0, 0))
			texture = _noTexture;
		else
			texture = _tilesTextures[tilesName];

		vertex->resize(height * width * 4);
		for (size_t no = 0; no < _map[id].data.size(); no++)
		{
			if (layer->second.data[no] == 0) continue; //if empty tile

			vertex->operator[]((no * 4) + 0).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight));
			vertex->operator[]((no * 4) + 1).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight));
			vertex->operator[]((no * 4) + 2).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));
			vertex->operator[]((no * 4) + 3).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));

			sf::IntRect rect;
			if (texture == _noTexture)
				rect = TilesHelper::GetTileRect(sf::Vector2u(16, 16), 16, 16, 0);
			else
				rect = TilesHelper::GetTileRect(texture->getSize(), layer->second.tileWidth, layer->second.tileHeight, layer->second.data[no] - 1);

			vertex->operator[]((no * 4) + 0).texCoords = sf::Vector2f((float)rect.left, (float)rect.top);
			vertex->operator[]((no * 4) + 1).texCoords = sf::Vector2f((float)rect.left + (float)rect.width, (float)rect.top);
			vertex->operator[]((no * 4) + 2).texCoords = sf::Vector2f((float)rect.left + (float)rect.width, (float)rect.top + (float)rect.height);
			vertex->operator[]((no * 4) + 3).texCoords = sf::Vector2f((float)rect.left, (float)rect.top + (float)rect.height);

			vertex->operator[]((no * 4) + 0).color = opacity;
			vertex->operator[]((no * 4) + 1).color = opacity;
			vertex->operator[]((no * 4) + 2).color = opacity;
			vertex->operator[]((no * 4) + 3).color = opacity;
		}
		_layerTransform[id].setPosition(offsetX, offsetY);
	}
}

template<typename T>
void GameMap<T>::PrepareActionMapLayer()
{
	auto vertex = &_actionMapVertices;
	vertex->setPrimitiveType(sf::Quads);

	auto layer = &_actionMap;
	if (layer->visible == false)
	{
		vertex->resize(0);
		return;
	}

	auto height = layer->height;
	auto width = layer->width;
	auto tileWidth = layer->tileWidth;
	auto tileHeight = layer->tileHeight;

	auto tilesName = layer->tilesName;
	sf::Color opacity = sf::Color(255, 255, 255, (sf::Uint8)(layer->opacity * 255));

	sf::Texture* texture = nullptr;
	if (_tilesTextures.find(tilesName) == _tilesTextures.end())
		texture = _noTexture;
	else if (_tilesTextures[tilesName]->getSize() == sf::Vector2u(0, 0))
		texture = _noTexture;
	else
		texture = _tilesTextures[tilesName];

	vertex->resize(height * width * 4);
	for (size_t no = 0; no < _actionMap.data.size(); no++)
	{
		if (layer->data[no] == 0) continue; //if empty tile

		vertex->operator[]((no * 4) + 0).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight));
		vertex->operator[]((no * 4) + 1).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight));
		vertex->operator[]((no * 4) + 2).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));
		vertex->operator[]((no * 4) + 3).position = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));

		sf::IntRect rect;
		if (texture == _noTexture)
			rect = TilesHelper::GetTileRect(sf::Vector2u(16, 16), 16, 16, 0);
		else
			rect = TilesHelper::GetTileRect(texture->getSize(), layer->tileWidth, layer->tileHeight, layer->data[no] - 1);

		vertex->operator[]((no * 4) + 0).texCoords = sf::Vector2f((float)rect.left, (float)rect.top);
		vertex->operator[]((no * 4) + 1).texCoords = sf::Vector2f((float)rect.left + (float)rect.width, (float)rect.top);
		vertex->operator[]((no * 4) + 2).texCoords = sf::Vector2f((float)rect.left + (float)rect.width, (float)rect.top + (float)rect.height);
		vertex->operator[]((no * 4) + 3).texCoords = sf::Vector2f((float)rect.left, (float)rect.top + (float)rect.height);

		vertex->operator[]((no * 4) + 0).color = opacity;
		vertex->operator[]((no * 4) + 1).color = opacity;
		vertex->operator[]((no * 4) + 2).color = opacity;
		vertex->operator[]((no * 4) + 3).color = opacity;
	}
	_actionMapTransform.setPosition(layer->offsetX, layer->offsetY);
}

template<typename T>
void GameMap<T>::PrepareActionMapGrid()
{
	_actionMapTransform.setPosition(_actionMap.offsetX, _actionMap.offsetY);

	_actionMapGrid.setPrimitiveType(sf::Lines);
	_actionMapGrid.resize(_actionMap.height * _actionMap.width * 8);

	auto width = _actionMap.width;
	auto tileWidth = _actionMap.tileWidth;
	auto tileHeight = _actionMap.tileHeight;
	for (size_t no = 0; no < _actionMap.data.size(); no++)
	{
		auto pos1 = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight));
		auto pos2 = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight));
		auto pos3 = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth + tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));
		auto pos4 = sf::Vector2f((float)(((uint32_t)no % width) * tileWidth), (float)(((uint32_t)no / width) * tileHeight + tileHeight));

		_actionMapGrid[(no * 8) + 0].position = pos1;
		_actionMapGrid[(no * 8) + 1].position = pos2;
		_actionMapGrid[(no * 8) + 2].position = pos2;
		_actionMapGrid[(no * 8) + 3].position = pos3;
		_actionMapGrid[(no * 8) + 4].position = pos3;
		_actionMapGrid[(no * 8) + 5].position = pos4;
		_actionMapGrid[(no * 8) + 6].position = pos4;
		_actionMapGrid[(no * 8) + 7].position = pos1;

		_actionMapGrid[(no * 8) + 0].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 1].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 2].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 3].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 4].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 5].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 6].color = _actionMapGridColor;
		_actionMapGrid[(no * 8) + 7].color = _actionMapGridColor;
	}
}

template<typename T>
void GameMap<T>::SetLayerVertexOpacity(unsigned int layerId, float opacity)
{
	auto layer = _layerVertices.find(layerId);
	if (layer == _layerVertices.end()) return;

	sf::Color color = sf::Color(255, 255, 255, (sf::Uint8)(opacity * 255));
	for (size_t no = 0; no < layer->second.getVertexCount(); no++)
		layer->second[no].color = color;
}

template<typename T>
void GameMap<T>::SetLayerVertexOffset(unsigned int layerId, const sf::Vector2f& offset)
{
	auto layer = _layerTransform.find(layerId);
	if (layer == _layerTransform.end()) return;

	layer->second.setPosition(offset);
}


#pragma region TemplateImplementationSigned
template void GameMap<int>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<int>::GameMap();
template GameMap<int>::~GameMap();
template void GameMap<int>::PrepareFrame();
template bool GameMap<int>::LoadFromFile(const std::string& path);
template void GameMap<int>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<int>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<int>::GetMapSize() const;
template unsigned int GameMap<int>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<int>::GetLayersIds() const;
template std::vector<std::string> GameMap<int>::GetLayersTilesNames() const;
template bool GameMap<int>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<int>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<int>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<int>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<int>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<int>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<int>::GetPathfindingPoints() const;
template const sf::Color& GameMap<int>::GetActionMapGridColor() const;
template bool GameMap<int>::GetActionMapGridVisibility() const;
template void GameMap<int>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<int>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<int>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<int>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<int>::ApplyLayerVisibility(bool visibility);
template void GameMap<int>::ApplyLayerOpacity(float opacity);
template void GameMap<int>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<int>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<int>::SetActionMapVisibility(bool visibility);
template void GameMap<int>::SetActionMapOpacity(float opacity);
template void GameMap<int>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<int>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<int>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<int>::SetActionMapGridVisibility(bool visible);
template void GameMap<int>::ToggleGridVisibility();
template void GameMap<int>::ToggleActionMapVisibility();

template void GameMap<char>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<char>::GameMap();
template GameMap<char>::~GameMap();
template void GameMap<char>::PrepareFrame();
template bool GameMap<char>::LoadFromFile(const std::string& path);
template void GameMap<char>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<char>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<char>::GetMapSize() const;
template unsigned int GameMap<char>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<char>::GetLayersIds() const;
template std::vector<std::string> GameMap<char>::GetLayersTilesNames() const;
template bool GameMap<char>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<char>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<char>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<char>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<char>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<char>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<char>::GetPathfindingPoints() const;
template const sf::Color& GameMap<char>::GetActionMapGridColor() const;
template bool GameMap<char>::GetActionMapGridVisibility() const;
template void GameMap<char>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<char>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<char>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<char>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<char>::ApplyLayerVisibility(bool visibility);
template void GameMap<char>::ApplyLayerOpacity(float opacity);
template void GameMap<char>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<char>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<char>::SetActionMapVisibility(bool visibility);
template void GameMap<char>::SetActionMapOpacity(float opacity);
template void GameMap<char>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<char>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<char>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<char>::SetActionMapGridVisibility(bool visible);
template void GameMap<char>::ToggleGridVisibility();
template void GameMap<char>::ToggleActionMapVisibility();

template void GameMap<short>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<short>::GameMap();
template GameMap<short>::~GameMap();
template void GameMap<short>::PrepareFrame();
template bool GameMap<short>::LoadFromFile(const std::string& path);
template void GameMap<short>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<short>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<short>::GetMapSize() const;
template unsigned int GameMap<short>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<short>::GetLayersIds() const;
template std::vector<std::string> GameMap<short>::GetLayersTilesNames() const;
template bool GameMap<short>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<short>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<short>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<short>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<short>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<short>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<short>::GetPathfindingPoints() const;
template const sf::Color& GameMap<short>::GetActionMapGridColor() const;
template bool GameMap<short>::GetActionMapGridVisibility() const;
template void GameMap<short>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<short>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<short>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<short>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<short>::ApplyLayerVisibility(bool visibility);
template void GameMap<short>::ApplyLayerOpacity(float opacity);
template void GameMap<short>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<short>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<short>::SetActionMapVisibility(bool visibility);
template void GameMap<short>::SetActionMapOpacity(float opacity);
template void GameMap<short>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<short>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<short>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<short>::SetActionMapGridVisibility(bool visible);
template void GameMap<short>::ToggleGridVisibility();
template void GameMap<short>::ToggleActionMapVisibility();
#pragma endregion

#pragma region TemplateImplementationUnsigned
template void GameMap<unsigned int>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned int>::GameMap();
template GameMap<unsigned int>::~GameMap();
template void GameMap<unsigned int>::PrepareFrame();
template bool GameMap<unsigned int>::LoadFromFile(const std::string& path);
template void GameMap<unsigned int>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<unsigned int>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<unsigned int>::GetMapSize() const;
template unsigned int GameMap<unsigned int>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<unsigned int>::GetLayersIds() const;
template std::vector<std::string> GameMap<unsigned int>::GetLayersTilesNames() const;
template bool GameMap<unsigned int>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<unsigned int>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<unsigned int>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<unsigned int>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<unsigned int>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<unsigned int>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<unsigned int>::GetPathfindingPoints() const;
template const sf::Color& GameMap<unsigned int>::GetActionMapGridColor() const;
template bool GameMap<unsigned int>::GetActionMapGridVisibility() const;
template void GameMap<unsigned int>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned int>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned int>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<unsigned int>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<unsigned int>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned int>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned int>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<unsigned int>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<unsigned int>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned int>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned int>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<unsigned int>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<unsigned int>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<unsigned int>::SetActionMapGridVisibility(bool visible);
template void GameMap<unsigned int>::ToggleGridVisibility();
template void GameMap<unsigned int>::ToggleActionMapVisibility();

template void GameMap<unsigned char>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned char>::GameMap();
template GameMap<unsigned char>::~GameMap();
template void GameMap<unsigned char>::PrepareFrame();
template bool GameMap<unsigned char>::LoadFromFile(const std::string& path);
template void GameMap<unsigned char>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<unsigned char>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<unsigned char>::GetMapSize() const;
template unsigned int GameMap<unsigned char>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<unsigned char>::GetLayersIds() const;
template std::vector<std::string> GameMap<unsigned char>::GetLayersTilesNames() const;
template bool GameMap<unsigned char>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<unsigned char>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<unsigned char>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<unsigned char>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<unsigned char>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<unsigned char>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<unsigned char>::GetPathfindingPoints() const;
template const sf::Color& GameMap<unsigned char>::GetActionMapGridColor() const;
template bool GameMap<unsigned char>::GetActionMapGridVisibility() const;
template void GameMap<unsigned char>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned char>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned char>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<unsigned char>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<unsigned char>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned char>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned char>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<unsigned char>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<unsigned char>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned char>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned char>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<unsigned char>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<unsigned char>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<unsigned char>::SetActionMapGridVisibility(bool visible);
template void GameMap<unsigned char>::ToggleGridVisibility();
template void GameMap<unsigned char>::ToggleActionMapVisibility();

template void GameMap<unsigned short>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned short>::GameMap();
template GameMap<unsigned short>::~GameMap();
template void GameMap<unsigned short>::PrepareFrame();
template bool GameMap<unsigned short>::LoadFromFile(const std::string& path);
template void GameMap<unsigned short>::SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
template bool GameMap<unsigned short>::AutoSetTilesTextures(TexturesManager* manager);
template const sf::Vector2u& GameMap<unsigned short>::GetMapSize() const;
template unsigned int GameMap<unsigned short>::GetNoOfLayers() const;
template const std::vector<unsigned int>& GameMap<unsigned short>::GetLayersIds() const;
template std::vector<std::string> GameMap<unsigned short>::GetLayersTilesNames() const;
template bool GameMap<unsigned short>::GetLayerVisibility(unsigned int layerId) const;
template float GameMap<unsigned short>::GetLayerOpacity(unsigned int layerId) const;
template sf::Vector2u GameMap<unsigned short>::GetLayerSize(unsigned int layerId) const;
template sf::Vector2f GameMap<unsigned short>::GetLayerOffset(unsigned int layerId) const;
template std::string GameMap<unsigned short>::GetLayerTilesName(unsigned int layerId) const;
template const MapLayerModel<unsigned char>* GameMap<unsigned short>::GetActionMap() const;
template const std::vector<sf::Vector2f>& GameMap<unsigned short>::GetPathfindingPoints() const;
template const sf::Color& GameMap<unsigned short>::GetActionMapGridColor() const;
template bool GameMap<unsigned short>::GetActionMapGridVisibility() const;
template void GameMap<unsigned short>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned short>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned short>::SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
template void GameMap<unsigned short>::SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
template void GameMap<unsigned short>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned short>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned short>::ApplyLayerOffset(const sf::Vector2f& offset);
template void GameMap<unsigned short>::ApplyLayerTilesName(const std::string& tilesName);
template void GameMap<unsigned short>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned short>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned short>::SetActionMapOffset(const sf::Vector2f& offset);
template void GameMap<unsigned short>::SetActionMapTilesName(const std::string& tilesName);
template void GameMap<unsigned short>::SetActionMapGridColor(const sf::Color& col);
template void GameMap<unsigned short>::SetActionMapGridVisibility(bool visible);
template void GameMap<unsigned short>::ToggleGridVisibility();
template void GameMap<unsigned short>::ToggleActionMapVisibility();
#pragma endregion
