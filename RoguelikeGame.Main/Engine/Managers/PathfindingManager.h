#pragma once

#include <vector>
#include <list>
#include <unordered_map>

#include "../Managers/CollisionsManager.h"
#include "../Utilities/Utilities.h"

typedef std::unordered_map<Vector2MapKey<float>, sf::Vector2f, Vector2MapKeyHasher<float>> Paths;

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
	Paths SolveDijkstras(Cell* startCell);
public:
	PathfindingManager();
	~PathfindingManager();

	void GenerateBaseGraph(const std::vector<sf::Vector2f>& points, CollisionsManager* collisions);
	std::list<sf::Vector2f> GetNodesInSight(const sf::Vector2f& start, CollisionsManager* collisions);

	//A* algh
	std::vector<sf::Vector2f> GetAStarPath(const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions);

	//Dijkstra's algh
	Paths GetDijkstrasPath(const sf::Vector2f& startPos, CollisionsManager* collisions);
	sf::Vector2f GetClosestNode(const Paths paths, const sf::Vector2f& startPos, CollisionsManager* collisions);
	sf::Vector2f GetClosestVisibleNodeTo(const Paths paths, const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions);
};

