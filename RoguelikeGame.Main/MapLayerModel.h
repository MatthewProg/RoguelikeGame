#pragma once

#include <string>
#include <vector>

#include "SFML/Graphics/Texture.hpp"

template<typename T>
class MapLayerModel
{
private:
public:
	MapLayerModel();
	~MapLayerModel();

	unsigned int id;
	unsigned int height;
	unsigned int width;

	unsigned int tileHeight;
	unsigned int tileWidth;

	float offsetX;
	float offsetY;

	bool visible;

	float opacity;

	std::string tilesName;

	std::vector<T> data;
};

template<typename T>
inline MapLayerModel<T>::MapLayerModel()
{
	id = 0;
	height = 0;
	width = 0;
	offsetX = 0;
	offsetY = 0;
	tileHeight = 16;
	tileWidth = 16;
	visible = true;
	opacity = 1;
	tilesName = "";
	data = std::vector<T>();
	data.clear();
}

template<typename T>
inline MapLayerModel<T>::~MapLayerModel()
{
}
