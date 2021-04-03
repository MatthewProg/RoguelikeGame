#include "CollisionsManager.h"

CollisionsManager::CollisionsManager()
{
	_logger = Logger::GetInstance();
	_maps.clear();
	_edges.clear();
	_edgesLines.resize(0);
	_edgesLines.setPrimitiveType(sf::PrimitiveType::Lines);
	_linesColor = sf::Color::White;
	_showCollisionLines = false;
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

	_sumMap.data.resize((size_t)_sumMap.height * (size_t)_sumMap.width, false);
	for (auto it = _maps.begin(); it != _maps.end(); it++)
		for (size_t i = 0; i < it->data.size(); i++)
			if (it->data[i])
				_sumMap.data[(i / it->width) * _sumMap.width + (i % it->width)] = true;
}

void CollisionsManager::CovertTilesIntoEdges()
{
	_edges.clear();

	struct Cell
	{
		size_t edge_id[4] = { 0,0,0,0 };
		bool edge_exist[4] = { false,false,false,false };
		bool exist = false;
	};
	std::vector<Cell> grid((size_t)_sumMap.width * (size_t)_sumMap.height);

	for (size_t i = 0; i < grid.size(); i++)
	{
		grid[i].exist = _sumMap.data[i];
		for (int j = 0; j < 4; j++)
		{
			grid[i].edge_exist[j] = false;
			grid[i].edge_id[j] = 0;
		}
	}

	// Iterate through region from top left to bottom right
	for (unsigned int x = 0; x < _sumMap.width; x++)
		for (unsigned y = 0; y < _sumMap.height; y++)
		{
			// Create some convenient indices
			int i = y * _sumMap.width + x;			// This
			int n = (y - 1) * _sumMap.width + x;	// Northern Neighbour
			int s = (y + 1) * _sumMap.width + x;	// Southern Neighbour
			int w = y * _sumMap.width + (x - 1);	// Western Neighbour
			int e = y * _sumMap.width + (x + 1);	// Eastern Neighbour

			// If this cell exists, check if it needs edges
			if (grid[i].exist)
			{
				if (x == 0 || !grid[w].exist) //Western neighbour or map end
				{
					if (y == 0 || grid[n].edge_exist[0] == false)
					{
						// Northern neighbour does not have one, so create one
						std::tuple<sf::Vector2f, sf::Vector2f> edge;

						float posX = (x * _sumMap.tileWidth) + _sumMap.offsetX;
						float posY = (y * _sumMap.tileHeight) + _sumMap.offsetY;

						std::get<0>(edge) = sf::Vector2f(posX, posY); //Start pos
						std::get<1>(edge) = sf::Vector2f(posX, posY + _sumMap.tileHeight); //End pos

						// Add edge to pool
						size_t edge_id = _edges.size();
						_edges.push_back(edge);

						// Update tile information with edge information
						grid[i].edge_id[0] = edge_id;
						grid[i].edge_exist[0] = true;
					}
					else
					{
						// Northern neighbour has a western edge, so grow it downwards
						std::get<1>(_edges[grid[n].edge_id[0]]).y += _sumMap.tileHeight;
						grid[i].edge_id[0] = grid[n].edge_id[0];
						grid[i].edge_exist[0] = true;
					}
				}

				if (x == _sumMap.width - 1 || !grid[e].exist) //Eastern neignbour or map end
				{
					if (y == 0 || !grid[n].edge_exist[1])
					{
						// Northern neighbour does not have one, so create one
						std::tuple<sf::Vector2f, sf::Vector2f> edge;

						float posX = ((x + 1) * _sumMap.tileWidth) + _sumMap.offsetX;
						float posY = (y * _sumMap.tileHeight) + _sumMap.offsetY;

						std::get<0>(edge) = sf::Vector2f(posX, posY); //Start pos
						std::get<1>(edge) = sf::Vector2f(posX, posY + _sumMap.tileHeight); //End pos

						// Add edge to Polygon Pool
						size_t edge_id = _edges.size();
						_edges.push_back(edge);

						// Update tile information with edge information
						grid[i].edge_id[1] = edge_id;
						grid[i].edge_exist[1] = true;			
					}
					else
					{
						// Northern neighbour has one, so grow it downwards
						std::get<1>(_edges[grid[n].edge_id[1]]).y += _sumMap.tileHeight;
						grid[i].edge_id[1] = grid[n].edge_id[1];
						grid[i].edge_exist[1] = true;
					}
				}

				if (y == 0 || !grid[n].exist) //Northern neignbour or map end
				{
					if (x == 0 || !grid[w].edge_exist[2])
					{
						// Western neighbour does not have one, so create one
						std::tuple<sf::Vector2f, sf::Vector2f> edge;

						float posX = (x * _sumMap.tileWidth) + _sumMap.offsetX;
						float posY = (y * _sumMap.tileHeight) + _sumMap.offsetY;

						std::get<0>(edge) = sf::Vector2f(posX, posY); //Start pos
						std::get<1>(edge) = sf::Vector2f(posX + _sumMap.tileWidth, posY); //End pos

						// Add edge to Polygon Pool
						size_t edge_id = _edges.size();
						_edges.push_back(edge);

						// Update tile information with edge information
						grid[i].edge_id[2] = edge_id;
						grid[i].edge_exist[2] = true;				
					}
					else
					{
						// Western neighbour has one, so grow it eastwards
						std::get<1>(_edges[grid[w].edge_id[2]]).x += _sumMap.tileWidth;
						grid[i].edge_id[2] = grid[w].edge_id[2];
						grid[i].edge_exist[2] = true;
					}
				}

				if (y == _sumMap.height - 1 || !grid[s].exist) //Southern neignbour or map end
				{
					if (x == 0 || !grid[w].edge_exist[3])
					{
						// Western neighbour does not have one, so I need to create one
						std::tuple<sf::Vector2f, sf::Vector2f> edge;

						float posX = (x * _sumMap.tileWidth) + _sumMap.offsetX;
						float posY = ((y + 1) * _sumMap.tileHeight) + _sumMap.offsetY;

						std::get<0>(edge) = sf::Vector2f(posX, posY); //Start pos
						std::get<1>(edge) = sf::Vector2f(posX + _sumMap.tileWidth, posY); //End pos

						// Add edge to Polygon Pool
						size_t edge_id = _edges.size();
						_edges.push_back(edge);

						// Update tile information with edge information
						grid[i].edge_id[3] = edge_id;
						grid[i].edge_exist[3] = true;		
					}
					else
					{
						// Western neighbour has one, so grow it eastwards
						std::get<1>(_edges[grid[w].edge_id[3]]).x += _sumMap.tileWidth;
						grid[i].edge_id[3] = grid[w].edge_id[3];
						grid[i].edge_exist[3] = true;
					}
				}
			}
		}

		//Pass points to VertexArray
		_edgesLines.clear();
		_edgesLines.setPrimitiveType(sf::PrimitiveType::Lines);
		_edgesLines.resize(_edges.size() * 2);
		for (size_t i = 0; i < _edges.size(); i++)
		{
			_edgesLines[(i * 2) + 0].position = std::get<0>(_edges[i]);
			_edgesLines[(i * 2) + 1].position = std::get<1>(_edges[i]);

			_edgesLines[(i * 2) + 0].color = _linesColor;
			_edgesLines[(i * 2) + 1].color = _linesColor;
		}
}

const std::vector<std::tuple<sf::Vector2f, sf::Vector2f>>* CollisionsManager::GetEdges() const
{
	return &_edges;
}

void CollisionsManager::SetCollisionLinesColor(const sf::Color& color)
{
	_linesColor = color;
	for (size_t i = 0; i < _edgesLines.getVertexCount(); i++)
		_edgesLines[i].color = color;
}

void CollisionsManager::SetCollisionLinesVisibility(bool visible)
{
	_showCollisionLines = visible;
}

void CollisionsManager::ToggleCollisionLinesVisibility()
{
	std::string status = (!_showCollisionLines) ? "true" : "false";
	_logger->Log(Logger::LogType::DEBUG, "Show map collision lines: " + status);
	SetCollisionLinesVisibility(!_showCollisionLines);
}

sf::Color CollisionsManager::GetCollisionLinesColor() const
{
	return _linesColor;
}

bool CollisionsManager::GetCollisionLinesVisibility() const
{
	return _showCollisionLines;
}

bool CollisionsManager::CheckTileCollision(const sf::FloatRect& rect) const
{
	return CollisionHelper::CheckTileCollision(rect, &_sumMap);
}

bool CollisionsManager::CheckCircleCollision(const sf::Vector2f& center, float radius) const
{
	return CollisionHelper::CheckTileCollision(center, radius, &_sumMap);
}

sf::Vector2f CollisionsManager::GetCircleLimitPosition(const sf::Vector2f& startPos, const sf::Vector2f& endPos, float radius) const
{
	return CollisionHelper::GetTileLimitPosition(startPos, endPos, radius, &_sumMap);
}

sf::Vector2f CollisionsManager::GetLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos) const
{
	return CollisionHelper::GetTileLimitPosition(startPos, endPos, &_sumMap);
}

sf::Vector2f CollisionsManager::GetRayHitpoint(const sf::Vector2f& center, float angle, float raycastRange) const
{
	auto endPoint = MathHelper::GetPointFromAngle(center, angle, raycastRange);

	sf::Vector2f closest = endPoint;
	float currDst = raycastRange;
	for (auto& l : _edges)
	{
		auto hitpoint = MathHelper::GetLinesIntersection(center, endPoint, std::get<0>(l), std::get<1>(l));
		auto distance = MathHelper::GetDistanceBetweenPoints(center, hitpoint);
		if (distance < currDst)
		{
			currDst = distance;
			closest = hitpoint;
		}
	}
	return closest;
}

bool CollisionsManager::RaycastHitsPoint(const sf::Vector2f& startPos, const sf::Vector2f& endPos, float* distanceToHitpoint) const
{
	float precision = 0.05F;
	auto angle = MathHelper::GetAngleBetweenPoints(startPos, endPos);
	auto range = MathHelper::GetDistanceBetweenPoints(startPos, endPos);
	auto hitpoint = GetRayHitpoint(startPos, angle, range);
	*distanceToHitpoint = MathHelper::GetDistanceBetweenPoints(startPos, hitpoint);
	return (*distanceToHitpoint >= range - precision && *distanceToHitpoint <= range + precision);
}

const std::vector<MapLayerModel<bool>>* CollisionsManager::GetStoredMaps() const
{
	return &_maps;
}

const MapLayerModel<bool>* CollisionsManager::GetCommonMap() const
{
	return &_sumMap;
}

void CollisionsManager::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if(_showCollisionLines)
		target.draw(_edgesLines);
}
