#pragma once

#include <fstream>
#include <unordered_map>

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "Utilities.h"
#include "TilesHelper.h"
#include "MapLayerModel.h"
#include "TexturesManager.h"

template<typename T>
class GameMap : public sf::Drawable
{
private:
	std::unordered_map<unsigned int, MapLayerModel<T>> _map;
	std::vector<unsigned int> _layersIds;
	MapLayerModel<unsigned char> _actionMap;

	std::unordered_map<std::string, sf::Texture*> _tilesTextures;
	sf::Texture* _noTexture;

	std::unordered_map<unsigned int, sf::VertexArray> _layerVertices;
	std::unordered_map<unsigned int, sf::Transformable> _layerTransform;

	sf::VertexArray _actionMapVertices;
	sf::Transformable _actionMapTransform;

	sf::VertexArray _actionMapGrid;
	sf::Color _actionMapGridColor;
	bool _showGrid;

	sf::Vector2u _mapSize;

	Logger* _logger;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetLayerVertexOpacity(unsigned int layerId, float opacity);
	void SetLayerVertexOffset(unsigned int layerId, sf::Vector2f offset);

	void PrepareActionMapLayer();
	void PrepareActionMapGrid();
public:

	GameMap();
	~GameMap();

	bool LoadFromFile(std::string path);

	void PrepareFrame();
	void Update(bool tick);

	void SetTilesTexture(std::string tilesName, sf::Texture* texture);
	bool AutoSetTilesTextures(TexturesManager* manager);

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
	sf::Color GetActionMapGridColor();
	bool GetActionMapGridVisibility();
	
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
	void SetActionMapGridColor(sf::Color col);
	void SetActionMapGridVisibility(bool visible);

	void ToggleGridVisibility();
	void ToggleActionMapVisibility();
};

