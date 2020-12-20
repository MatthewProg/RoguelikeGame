#include "MapLayerModel.h"

template<typename T>
MapLayerModel<T>::MapLayerModel()
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
MapLayerModel<T>::~MapLayerModel()
{
}

template MapLayerModel<int>::MapLayerModel();
template MapLayerModel<short>::MapLayerModel();
template MapLayerModel<char>::MapLayerModel();
template MapLayerModel<unsigned int>::MapLayerModel();
template MapLayerModel<unsigned short>::MapLayerModel();
template MapLayerModel<unsigned char>::MapLayerModel();

template MapLayerModel<int>::~MapLayerModel();
template MapLayerModel<short>::~MapLayerModel();
template MapLayerModel<char>::~MapLayerModel();
template MapLayerModel<unsigned int>::~MapLayerModel();
template MapLayerModel<unsigned short>::~MapLayerModel();
template MapLayerModel<unsigned char>::~MapLayerModel();