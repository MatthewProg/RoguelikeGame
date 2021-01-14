#pragma once

#include <vector>
#include <list>

#include "CollisionsManager.h"

class PathfindingManager
{
private:
	struct Cell
	{
		bool visited = false;

		sf::Vector2f pos;
		float localGoal = INFINITY;
		float globalGoal = INFINITY;

		Cell* parent = nullptr;
		std::map<Cell*, float> neighbours;
	};

	std::list<Cell> _baseGraph;

	std::vector<sf::Vector2f> SolveAStar(Cell* startCell, Cell* endCell);
public:
	PathfindingManager();
	~PathfindingManager();

	void GenerateBaseGraph(const std::vector<sf::Vector2f>& points, CollisionsManager* collisions);
	std::vector<sf::Vector2f> GetPath(const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions);
};

