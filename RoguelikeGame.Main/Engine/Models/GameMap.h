#pragma once

#include <fstream>
#include <unordered_map>

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "nlohmann/json.hpp"

#include "../Core/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Helpers/TilesHelper.h"
#include "../Models/MapLayerModel.h"
#include "../Managers/TexturesManager.h"

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

	std::vector<sf::Vector2f> _pathfingingPoints;

	sf::VertexArray _actionMapGrid;
	sf::Color _actionMapGridColor;
	bool _showGrid;

	sf::Vector2u _mapSize;

	Logger* _logger;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetLayerVertexOpacity(unsigned int layerId, float opacity);
	void SetLayerVertexOffset(unsigned int layerId, const sf::Vector2f& offset);

	void PrepareActionMapLayer();
	void PrepareActionMapGrid();
public:

	GameMap();
	~GameMap() override = default;

	bool LoadFromFile(const std::string& path);

	void PrepareFrame();

	void SetTilesTexture(const std::string& tilesName, sf::Texture* texture);
	bool AutoSetTilesTextures(TexturesManager* manager);

	//Map getters
	const sf::Vector2u& GetMapSize() const;
	size_t GetNoOfLayers() const;
	const std::vector<unsigned int>& GetLayersIds() const;
	std::vector<std::string> GetLayersTilesNames() const;
	bool GetLayerVisibility(unsigned int layerId) const;
	float GetLayerOpacity(unsigned int layerId) const;
	sf::Vector2u GetLayerSize(unsigned int layerId) const;
	sf::Vector2f GetLayerOffset(unsigned int layerId) const;
	std::string GetLayerTilesName(unsigned int layerId) const;
	const MapLayerModel<unsigned char>* GetActionMap() const;
	const std::vector<sf::Vector2f>& GetPathfindingPoints() const;
	const sf::Color& GetActionMapGridColor() const;
	bool GetActionMapGridVisibility() const;
	
	//Map setters
	void SetLayerVisibility(unsigned int layerId, bool visibility);
	void SetLayerOpacity(unsigned int layerId, float opacity);
	void SetLayerOffset(unsigned int layerId, const sf::Vector2f& offset);
	void SetLayerTilesName(unsigned int layerId, const std::string& tilesName);
	void ApplyLayerVisibility(bool visibility);
	void ApplyLayerOpacity(float opacity);
	void ApplyLayerOffset(const sf::Vector2f& offset);
	void ApplyLayerTilesName(const std::string& tilesName);
	void SetActionMapVisibility(bool visibility);
	void SetActionMapOpacity(float opacity);
	void SetActionMapOffset(const sf::Vector2f& offset);
	void SetActionMapTilesName(const std::string& tilesName);
	void SetActionMapGridColor(const sf::Color& col);
	void SetActionMapGridVisibility(bool visible);

	void ToggleGridVisibility();
	void ToggleActionMapVisibility();
};

