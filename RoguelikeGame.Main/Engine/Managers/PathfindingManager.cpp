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

	while (!listNotTestedNodes.empty()/* && currentCell != endCell*/)
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

std::vector<sf::Vector2f> PathfindingManager::GetPath(const sf::Vector2f& startPos, const sf::Vector2f& endPos, CollisionsManager* collisions)
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
	last--;
	for (auto cell = _baseGraph.begin(); cell != last; cell++)
	{
		for (auto check = last; check != _baseGraph.end(); check++)
		{
			float distance = 0;
			if (collisions->RaycastHitsPoint(check->pos, cell->pos, &distance))
			{
				cell->neighbours[&(*check)] = distance;
				check->neighbours[&(*cell)] = distance;
			}
		}
	}

	auto output = SolveAStar(&(*last), &(*last++));

	//Remove addresses from neighbours and points
	for (auto mapped : last->neighbours)
		mapped.first->neighbours.erase(mapped.first->neighbours.find(&(*last)));

	last--;

	for (auto mapped : last->neighbours) 
		mapped.first->neighbours.erase(mapped.first->neighbours.find(&(*last)));

	_baseGraph.erase(last , _baseGraph.end());

	return output;
}
