#include "PathfindingManager.h"

std::vector<sf::Vector2f> PathfindingManager::SolveAStar(Cell* startCell, Cell* endCell)
{
	std::vector<sf::Vector2f> output;

	//Reset alghoritm vars
	for(auto it = _baseGraph.begin();it!=_baseGraph.end();it++)
	{
		it->visited = false;
		it->globalGoal = INFINITY;
		it->localGoal = INFINITY;
		it->parent = nullptr;
	}

	auto heuristic = [](Cell* a, Cell* b)
	{
		return MathHelper::GetDistanceBetweenPoints(a->pos, b->pos);
	};

	// Setup starting conditions
	Cell* currentCell = startCell;
	startCell->localGoal = 0.0f;
	startCell->globalGoal = heuristic(startCell, endCell);

	std::list<Cell*> listNotTestedNodes;
	listNotTestedNodes.push_back(startCell);

	while (!listNotTestedNodes.empty())
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Cell* lhs, const Cell* rhs) { return lhs->globalGoal < rhs->globalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		currentCell = listNotTestedNodes.front();
		currentCell->visited = true;


		// Check each of this node's neighbours
		for (auto nodeNeighbour : currentCell->neighbours)
		{
			if (!nodeNeighbour.first->visited)
				listNotTestedNodes.push_back(nodeNeighbour.first);

			//Potential lowest parent distance
			float fPossiblyLowerGoal = currentCell->localGoal + nodeNeighbour.second;

			if (fPossiblyLowerGoal < nodeNeighbour.first->localGoal)
			{
				nodeNeighbour.first->parent = currentCell;
				nodeNeighbour.first->localGoal = fPossiblyLowerGoal;

				nodeNeighbour.first->globalGoal = nodeNeighbour.first->localGoal + heuristic(nodeNeighbour.first, endCell);
			}
		}
	}

	Cell* p = endCell;
	while (p->parent != nullptr)
	{
		output.push_back(p->pos);
		p = p->parent;
	}

	return output;
}

Paths PathfindingManager::SolveDijkstras(Cell* startCell)
{
	std::vector<sf::Vector2f> output;

	//Reset alghoritm vars
	for (auto it = _baseGraph.begin(); it != _baseGraph.end(); it++)
	{
		it->visited = false;
		it->globalGoal = INFINITY;
		it->localGoal = INFINITY;
		it->parent = nullptr;
	}

	// Setup starting conditions
	Cell* currentCell = startCell;
	startCell->localGoal = 0.0f;
	startCell->globalGoal = 0.0f;

	std::list<Cell*> listNotTestedNodes;
	listNotTestedNodes.push_back(startCell);

	while (!listNotTestedNodes.empty())
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Cell* lhs, const Cell* rhs) { return lhs->globalGoal < rhs->globalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		currentCell = listNotTestedNodes.front();
		currentCell->visited = true;

		// Check each of this node's neighbours
		for (auto nodeNeighbour : currentCell->neighbours)
		{
			if (!nodeNeighbour.first->visited)
				listNotTestedNodes.push_back(nodeNeighbour.first);

			//Potential lowest parent distance
			float fPossiblyLowerGoal = currentCell->localGoal + nodeNeighbour.second;

			if (fPossiblyLowerGoal < nodeNeighbour.first->localGoal)
			{
				nodeNeighbour.first->parent = currentCell;
				nodeNeighbour.first->localGoal = fPossiblyLowerGoal;

				nodeNeighbour.first->globalGoal = nodeNeighbour.first->localGoal;
			}
		}
	}

	Paths paths;	
	for (auto it = _baseGraph.begin(); it != _baseGraph.end(); it++)
	{
		if (it->parent == nullptr) paths[Vector2MapKey<float>(it->pos)] = sf::Vector2f(INFINITY, INFINITY);
		else paths[Vector2MapKey<float>(it->pos)] = it->parent->pos;
	}
	return paths;
}

PathfindingManager::PathfindingManager()
{
	_baseGraph.clear();
}

PathfindingManager::~PathfindingManager()
{
}

void PathfindingManager::GenerateBaseGraph(const std::vector<sf::Vector2f>& points, CollisionsManager* collisions)
{
	_baseGraph.clear();

	for (auto point : points)
	{
		Cell c;
		c.visited = false;
		c.pos = point;
		c.parent = nullptr;
		c.globalGoal = INFINITY;
		c.localGoal = INFINITY;
		c.neighbours.clear();
		_baseGraph.push_back(c);
	}

	for (auto cell = _baseGraph.begin(); cell != _baseGraph.end(); cell++)
	{
		for (auto neighbour = _baseGraph.begin(); neighbour != _baseGraph.end(); neighbour++)
		{
			if (cell == neighbour) continue;

			float distance = 0;
			if (collisions->RaycastHitsPoint(cell->pos, neighbour->pos, &distance))
				cell->neighbours[&(*neighbour)] = distance;
		}
	}
}

std::list<sf::Vector2f> PathfindingManager::GetNodesInSight(const sf::Vector2f& start, CollisionsManager* collisions)
{
	std::list<sf::Vector2f> output;

	for (auto cell = _baseGraph.begin(); cell != _baseGraph.end(); cell++)
	{
		float distance = 0;
		if (collisions->RaycastHitsPoint(start, cell->pos, &distance))
			output.push_back(cell->pos);
	}
	
	return output;
}

std::vector<sf::Vector2f> PathfindingManager::GetAStarPath(const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions)
{
	Cell start, end;

	for (int i = 0; i < 2; i++)
	{
		Cell* init = nullptr;
		if (i == 0) init = &start;
		else init = &end;

		init->globalGoal = INFINITY;
		init->localGoal = INFINITY;
		init->neighbours.clear();
		init->parent = nullptr;
		init->visited = false;
		init->pos = (i == 0) ? startPos : endPos;
	}

	_baseGraph.push_back(start);
	_baseGraph.push_back(end);

	auto last = _baseGraph.end();
	last--;
	auto endIt = last;
	last--;
	auto startIt = last;

	for (auto cell = _baseGraph.begin(); cell != startIt; cell++)
	{
		for (auto check = startIt; check != _baseGraph.end(); check++)
		{
			float distance = 0;
			if (collisions->RaycastHitsPoint(check->pos, cell->pos, &distance))
			{
				cell->neighbours[&(*check)] = distance;
				check->neighbours[&(*cell)] = distance;
			}
		}
	}

	auto output = SolveAStar(&(*startIt), &(*endIt));

	//Remove addresses from neighbours and points
	for (auto mapped : startIt->neighbours)
		mapped.first->neighbours.erase(mapped.first->neighbours.find(&(*startIt)));
	for (auto mapped : endIt->neighbours)
		mapped.first->neighbours.erase(mapped.first->neighbours.find(&(*endIt)));

	_baseGraph.erase(startIt, _baseGraph.end());

	return output;
}

Paths PathfindingManager::GetDijkstrasPath(const sf::Vector2f& startPos, CollisionsManager* collisions)
{
	//Init
	Cell start;
	start.globalGoal = INFINITY;
	start.localGoal = INFINITY;
	start.neighbours.clear();
	start.parent = nullptr;
	start.visited = false;
	start.pos = startPos;

	_baseGraph.push_back(start);
	
	//Get var
	auto startCell = _baseGraph.end();
	startCell--;

	//Check connections with other nodes
	for (auto check = _baseGraph.begin(); check != startCell; check++)
	{
		float distance = 0;
		if (collisions->RaycastHitsPoint(check->pos, startCell->pos, &distance))
		{
			startCell->neighbours[&(*check)] = distance;
			check->neighbours[&(*startCell)] = distance;
		}
	}

	//Solve
	auto output = SolveDijkstras(&(*startCell));

	//Clean
	for (auto n : startCell->neighbours)
		n.first->neighbours.erase(n.first->neighbours.find(&(*startCell)));

	_baseGraph.erase(startCell);

	return output;
}

sf::Vector2f PathfindingManager::GetClosestNode(const Paths paths, const sf::Vector2f& startPos, CollisionsManager* collisions)
{
	std::list<std::tuple<sf::Vector2f, float>> nodesDistances;
	for (auto &p : paths)
	{
		std::tuple<sf::Vector2f, float> t(p.first, MathHelper::GetDistanceBetweenPoints(startPos, p.first));
		nodesDistances.push_back(t);
	}

	//Sort to make closest ones first
	auto sort = [](std::tuple<sf::Vector2f, float> first, std::tuple<sf::Vector2f, float> second)
	{
		return std::get<1>(first) < std::get<1>(second);
	};
	nodesDistances.sort(sort);

	//Check which hits
	for (auto p : nodesDistances)
	{
		float distance = 0.f;
		if (collisions->RaycastHitsPoint(startPos, std::get<0>(p), &distance))
			return std::get<0>(p);
	}
	
	return sf::Vector2f(INFINITY, INFINITY);
}

sf::Vector2f PathfindingManager::GetClosestVisibleNodeTo(const Paths paths, const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions)
{
	std::list<std::tuple<sf::Vector2f, float>> nodesDistances;
	for (auto& p : paths)
	{
		if (p.second.x == INFINITY) continue;
		std::tuple<sf::Vector2f, float> t(p.first, MathHelper::GetDistanceBetweenPoints(endPos, p.first));
		nodesDistances.push_back(t);
	}

	//Sort to make closest ones first
	auto sort = [](std::tuple<sf::Vector2f, float> first, std::tuple<sf::Vector2f, float> second)
	{
		return std::get<1>(first) < std::get<1>(second);
	};
	nodesDistances.sort(sort);

	//Check which hits
	for (auto p : nodesDistances)
	{
		float distance = 0.f;
		if (collisions->RaycastHitsPoint(startPos, std::get<0>(p), &distance))
			return std::get<0>(p);
	}

	return sf::Vector2f(INFINITY, INFINITY);
}
