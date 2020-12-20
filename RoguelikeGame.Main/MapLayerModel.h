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
