#include "GameMap.h"

template<typename T>
GameMap<T>::GameMap()
{
	_map.clear();
	_layersIds.clear();
	_actionMap.visible = false;
	_actionMap.opacity = 0.5;
	_actionMap.tilesName = "special";
	_logger = Logger::GetInstance();
	_noTexture.loadFromImage(Utilities::GetInstance()->NoTexture16x16());
}

template<typename T>
GameMap<T>::~GameMap()
{
}

template<typename T>
bool GameMap<T>::LoadFromFile(std::string path)
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

	_actionMap.height = doc["actionLayer"]["height"].get<unsigned int>();
	_actionMap.width = doc["actionLayer"]["width"].get<unsigned int>();
	_actionMap.offsetX = doc["actionLayer"]["x"].get<float>();
	_actionMap.offsetY = doc["actionLayer"]["y"].get<float>();
	_actionMap.tileHeight = doc["actionLayer"]["tileHeight"].get<unsigned int>();
	_actionMap.tileWidth = doc["actionLayer"]["tileWidth"].get<unsigned int>();
	for (auto& dat : doc["actionLayer"]["data"])
		_actionMap.data.push_back(dat.get<unsigned char>());

	std::sort(_layersIds.begin(), _layersIds.end());
	return true;
}

template<typename T>
bool GameMap<T>::SetTilesTexture(std::string tilesName, const sf::Image& img)
{
	if (!_tilesTextures[tilesName].loadFromImage(img))
	{
		_logger->Log(Logger::LogType::ERROR, "Unable to set tiles texture in game map!");
		return false;
	}
	return true;
}

template<typename T>
sf::Vector2u GameMap<T>::GetMapSize()
{
	return _mapSize;
}

template<typename T>
unsigned int GameMap<T>::GetNoOfLayers()
{
	return _layersIds.size();
}

template<typename T>
std::vector<unsigned int> GameMap<T>::GetLayersIds()
{
	return _layersIds;
}

template<typename T>
std::vector<std::string> GameMap<T>::GetLayersTilesNames()
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
bool GameMap<T>::GetLayerVisibility(unsigned int layerId)
{
	if (_map.find(layerId) != _map.end())
		return _map[layerId].visible;
	return false;
}

template<typename T>
float GameMap<T>::GetLayerOpacity(unsigned int layerId)
{
	if (_map.find(layerId) != _map.end())
		return _map[layerId].opacity;
	return 0.0f;
}

template<typename T>
sf::Vector2u GameMap<T>::GetLayerSize(unsigned int layerId)
{
	if (_map.find(layerId) != _map.end())
		return sf::Vector2u(_map[layerId].width, _map[layerId].height);
	return sf::Vector2u(0, 0);
}

template<typename T>
sf::Vector2f GameMap<T>::GetLayerOffset(unsigned int layerId)
{
	if (_map.find(layerId) != _map.end())
		return sf::Vector2f(_map[layerId].offsetX, _map[layerId].offsetY);
	return sf::Vector2f(0, 0);
}

template<typename T>
std::string GameMap<T>::GetLayerTilesName(unsigned int layerId)
{
	if (_map.find(layerId) != _map.end())
		return _map[layerId].tilesName;
	return std::string();
}

template<typename T>
MapLayerModel<unsigned char>* GameMap<T>::GetActionMap()
{
	return &_actionMap;
}

template<typename T>
void GameMap<T>::SetLayerVisibility(unsigned int layerId, bool visibility)
{
	if (_map.find(layerId) != _map.end())
		_map[layerId].visible = visibility;
	return;
}

template<typename T>
void GameMap<T>::SetLayerOpacity(unsigned int layerId, float opacity)
{
	float opa = opacity;
	if (opacity > 1) opa = 1;
	if (opacity < 0) opa = 0;

	if (_map.find(layerId) != _map.end())
		_map[layerId].opacity = opa;
	return;
}

template<typename T>
void GameMap<T>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset)
{
	if (_map.find(layerId) != _map.end())
	{
		_map[layerId].offsetX = offset.x;
		_map[layerId].offsetY = offset.y;
	}
	return;
}

template<typename T>
void GameMap<T>::SetLayerTilesName(unsigned int layerId, std::string tilesName)
{
	if (_map.find(layerId) != _map.end())
		_map[layerId].tilesName = tilesName;
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
		iter->second.opacity = opa;
}

template<typename T>
void GameMap<T>::ApplyLayerOffset(sf::Vector2f offset)
{
	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
	{
		iter->second.offsetX = offset.x;
		iter->second.offsetY = offset.y;
	}
}

template<typename T>
void GameMap<T>::ApplyLayerTilesName(std::string tilesName)
{
	for (auto iter = _map.begin(); iter != _map.end(); ++iter)
		iter->second.tilesName = tilesName;
}

template<typename T>
void GameMap<T>::SetActionMapVisibility(bool visibility)
{
	_actionMap.visible = visibility;
}

template<typename T>
void GameMap<T>::SetActionMapOpacity(float opacity)
{
	float opa = opacity;
	if (opacity > 1) opa = 1;
	if (opacity < 0) opa = 0;

	_actionMap.opacity = opa;
}

template<typename T>
void GameMap<T>::SetActionMapOffset(sf::Vector2f offset)
{
	_actionMap.offsetX = offset.x;
	_actionMap.offsetY = offset.y;
}

template<typename T>
void GameMap<T>::SetActionMapTilesName(std::string tilesName)
{
	_actionMap.tilesName = tilesName;
}

template<typename T>
void GameMap<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < _layersIds.size(); i++)
	{
		auto id = _layersIds[i];
		auto layer = _map.find(id);

		if (layer == _map.end()) continue;
		if (layer->second.visible == false) continue;

		auto height = layer->second.height;
		auto width = layer->second.width;
		auto offsetX = layer->second.offsetX;
		auto offsetY = layer->second.offsetY;
		auto tileWidth = layer->second.tileWidth;
		auto tileHeight = layer->second.tileHeight;

		auto opacity = layer->second.opacity;
		auto tilesName = layer->second.tilesName;

		sf::Sprite tile;
		tile.setColor(sf::Color(255, 255, 255, (255 * opacity)));
		auto texture = _tilesTextures.find(tilesName);

		if (texture == _tilesTextures.end())
			tile.setTexture(_noTexture);
		else if (texture->second.getSize() == sf::Vector2u(0, 0))
			tile.setTexture(_noTexture);
		else
			tile.setTexture(texture->second);

		for (size_t no = 0; no < layer->second.data.size(); no++)
		{
			if (layer->second.data[no] == 0) continue;

			tile.setTextureRect(TilesHelper::GetTileRect(texture->second.getSize(), tileWidth, tileHeight, layer->second.data[no]-1));
			tile.setPosition((no % width)*tileWidth + offsetX, (no / width)*tileHeight + offsetY);
			target.draw(tile);
		}
	}

	if (_actionMap.visible == false) return;

	sf::Sprite act;
	act.setColor(sf::Color(255, 255, 255, (255 * _actionMap.opacity)));

	auto texture = _tilesTextures.find(_actionMap.tilesName);

	if (texture == _tilesTextures.end())
		act.setTexture(_noTexture);
	else if (texture->second.getSize() == sf::Vector2u(0, 0))
		act.setTexture(_noTexture);
	else
		act.setTexture(texture->second);

	for (size_t no = 0; no < _actionMap.data.size(); no++)
	{
		if (_actionMap.data[no] == 0) continue;

		act.setTextureRect(TilesHelper::GetTileRect(texture->second.getSize(), _actionMap.tileWidth, _actionMap.tileHeight, _actionMap.data[no]-1));
		act.setPosition((no % _actionMap.width) * _actionMap.tileWidth + _actionMap.offsetX, (no / _actionMap.width) * _actionMap.tileHeight + _actionMap.offsetY);
		target.draw(act);
	}
}

#pragma region TemplateImplementationSigned
template void GameMap<int>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<int>::GameMap();
template GameMap<int>::~GameMap();
template bool GameMap<int>::LoadFromFile(std::string path);
template bool GameMap<int>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<int>::GetMapSize();
template unsigned int GameMap<int>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<int>::GetLayersIds();
template std::vector<std::string> GameMap<int>::GetLayersTilesNames();
template bool GameMap<int>::GetLayerVisibility(unsigned int layerId);
template float GameMap<int>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<int>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<int>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<int>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<int>::GetActionMap();
template void GameMap<int>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<int>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<int>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<int>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<int>::ApplyLayerVisibility(bool visibility);
template void GameMap<int>::ApplyLayerOpacity(float opacity);
template void GameMap<int>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<int>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<int>::SetActionMapVisibility(bool visibility);
template void GameMap<int>::SetActionMapOpacity(float opacity);
template void GameMap<int>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<int>::SetActionMapTilesName(std::string tilesName);

template void GameMap<char>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<char>::GameMap();
template GameMap<char>::~GameMap();
template bool GameMap<char>::LoadFromFile(std::string path);
template bool GameMap<char>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<char>::GetMapSize();
template unsigned int GameMap<char>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<char>::GetLayersIds();
template std::vector<std::string> GameMap<char>::GetLayersTilesNames();
template bool GameMap<char>::GetLayerVisibility(unsigned int layerId);
template float GameMap<char>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<char>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<char>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<char>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<char>::GetActionMap();
template void GameMap<char>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<char>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<char>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<char>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<char>::ApplyLayerVisibility(bool visibility);
template void GameMap<char>::ApplyLayerOpacity(float opacity);
template void GameMap<char>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<char>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<char>::SetActionMapVisibility(bool visibility);
template void GameMap<char>::SetActionMapOpacity(float opacity);
template void GameMap<char>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<char>::SetActionMapTilesName(std::string tilesName);

template void GameMap<short>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<short>::GameMap();
template GameMap<short>::~GameMap();
template bool GameMap<short>::LoadFromFile(std::string path);
template bool GameMap<short>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<short>::GetMapSize();
template unsigned int GameMap<short>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<short>::GetLayersIds();
template std::vector<std::string> GameMap<short>::GetLayersTilesNames();
template bool GameMap<short>::GetLayerVisibility(unsigned int layerId);
template float GameMap<short>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<short>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<short>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<short>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<short>::GetActionMap();
template void GameMap<short>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<short>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<short>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<short>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<short>::ApplyLayerVisibility(bool visibility);
template void GameMap<short>::ApplyLayerOpacity(float opacity);
template void GameMap<short>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<short>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<short>::SetActionMapVisibility(bool visibility);
template void GameMap<short>::SetActionMapOpacity(float opacity);
template void GameMap<short>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<short>::SetActionMapTilesName(std::string tilesName);
#pragma endregion

#pragma region TemplateImplementationUnsigned
template void GameMap<unsigned int>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned int>::GameMap();
template GameMap<unsigned int>::~GameMap();
template bool GameMap<unsigned int>::LoadFromFile(std::string path);
template bool GameMap<unsigned int>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<unsigned int>::GetMapSize();
template unsigned int GameMap<unsigned int>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<unsigned int>::GetLayersIds();
template std::vector<std::string> GameMap<unsigned int>::GetLayersTilesNames();
template bool GameMap<unsigned int>::GetLayerVisibility(unsigned int layerId);
template float GameMap<unsigned int>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<unsigned int>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<unsigned int>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<unsigned int>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<unsigned int>::GetActionMap();
template void GameMap<unsigned int>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned int>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned int>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<unsigned int>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<unsigned int>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned int>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned int>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<unsigned int>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<unsigned int>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned int>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned int>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<unsigned int>::SetActionMapTilesName(std::string tilesName);

template void GameMap<unsigned char>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned char>::GameMap();
template GameMap<unsigned char>::~GameMap();
template bool GameMap<unsigned char>::LoadFromFile(std::string path);
template bool GameMap<unsigned char>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<unsigned char>::GetMapSize();
template unsigned int GameMap<unsigned char>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<unsigned char>::GetLayersIds();
template std::vector<std::string> GameMap<unsigned char>::GetLayersTilesNames();
template bool GameMap<unsigned char>::GetLayerVisibility(unsigned int layerId);
template float GameMap<unsigned char>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<unsigned char>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<unsigned char>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<unsigned char>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<unsigned char>::GetActionMap();
template void GameMap<unsigned char>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned char>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned char>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<unsigned char>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<unsigned char>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned char>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned char>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<unsigned char>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<unsigned char>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned char>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned char>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<unsigned char>::SetActionMapTilesName(std::string tilesName);

template void GameMap<unsigned short>::draw(sf::RenderTarget& target, sf::RenderStates states) const;
template GameMap<unsigned short>::GameMap();
template GameMap<unsigned short>::~GameMap();
template bool GameMap<unsigned short>::LoadFromFile(std::string path);
template bool GameMap<unsigned short>::SetTilesTexture(std::string tilesName, const sf::Image& img);
template sf::Vector2u GameMap<unsigned short>::GetMapSize();
template unsigned int GameMap<unsigned short>::GetNoOfLayers();
template std::vector<unsigned int> GameMap<unsigned short>::GetLayersIds();
template std::vector<std::string> GameMap<unsigned short>::GetLayersTilesNames();
template bool GameMap<unsigned short>::GetLayerVisibility(unsigned int layerId);
template float GameMap<unsigned short>::GetLayerOpacity(unsigned int layerId);
template sf::Vector2u GameMap<unsigned short>::GetLayerSize(unsigned int layerId);
template sf::Vector2f GameMap<unsigned short>::GetLayerOffset(unsigned int layerId);
template std::string GameMap<unsigned short>::GetLayerTilesName(unsigned int layerId);
template MapLayerModel<unsigned char>* GameMap<unsigned short>::GetActionMap();
template void GameMap<unsigned short>::SetLayerVisibility(unsigned int layerId, bool visibility);
template void GameMap<unsigned short>::SetLayerOpacity(unsigned int layerId, float opacity);
template void GameMap<unsigned short>::SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
template void GameMap<unsigned short>::SetLayerTilesName(unsigned int layerId, std::string tilesName);
template void GameMap<unsigned short>::ApplyLayerVisibility(bool visibility);
template void GameMap<unsigned short>::ApplyLayerOpacity(float opacity);
template void GameMap<unsigned short>::ApplyLayerOffset(sf::Vector2f offset);
template void GameMap<unsigned short>::ApplyLayerTilesName(std::string tilesName);
template void GameMap<unsigned short>::SetActionMapVisibility(bool visibility);
template void GameMap<unsigned short>::SetActionMapOpacity(float opacity);
template void GameMap<unsigned short>::SetActionMapOffset(sf::Vector2f offset);
template void GameMap<unsigned short>::SetActionMapTilesName(std::string tilesName);
#pragma endregion
