#pragma once

#include <fstream>
#include <map>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Vector2.hpp"
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "Utilities.h"
#include "TilesHelper.h"
#include "MapLayerModel.h"

template<typename T>
class GameMap : public sf::Drawable
{
private:
	std::map<unsigned int, MapLayerModel<T>> _map;
	std::vector<unsigned int> _layersIds;
	MapLayerModel<unsigned char> _actionMap;

	std::map<std::string, sf::Texture> _tilesTextures;
	sf::Texture _noTexture;

	sf::Vector2u _mapSize;

	Logger* _logger;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:

	GameMap();
	~GameMap();

	bool LoadFromFile(std::string path);

	bool SetTilesTexture(std::string tilesName, const sf::Image& img);

	//Map getters
	sf::Vector2u GetMapSize();
	unsigned int GetNoOfLayers();
	std::vector<unsigned int> GetLayersIds();
	std::vector<std::string> GetLayersTilesNames();
	bool GetLayerVisibility(unsigned int layerId);
	float GetLayerOpacity(unsigned int layerId);
	sf::Vector2u GetLayerSize(unsigned int layerId);
	sf::Vector2f GetLayerOffset(unsigned int layerId);
	std::string GetLayerTilesName(unsigned int layerId);
	MapLayerModel<unsigned char>* GetActionMap();
	
	//Map setters
	void SetLayerVisibility(unsigned int layerId, bool visibility);
	void SetLayerOpacity(unsigned int layerId, float opacity);
	void SetLayerOffset(unsigned int layerId, sf::Vector2f offset);
	void SetLayerTilesName(unsigned int layerId, std::string tilesName);
	void ApplyLayerVisibility(bool visibility);
	void ApplyLayerOpacity(float opacity);
	void ApplyLayerOffset(sf::Vector2f offset);
	void ApplyLayerTilesName(std::string tilesName);
	void SetActionMapVisibility(bool visibility);
	void SetActionMapOpacity(float opacity);
	void SetActionMapOffset(sf::Vector2f offset);
	void SetActionMapTilesName(std::string tilesName);
};

