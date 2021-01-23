#include "EnemiesAI.h"

bool EnemiesAI::DirectLineOfSight(Enemy* source, sf::Vector2f* raycastHitpoint)
{
	if (source == nullptr || _target == nullptr) return false;

	//Get needed values
	auto rayPrecision = 2.F;
	auto targetCenter = ViewHelper::GetRectCenter(_target->GetCollisionBox());
	auto enemyCenter = ViewHelper::GetRectCenter(source->GetCollisionBox());
	auto angle = MathHelper::GetAngleBetweenPoints(enemyCenter, targetCenter);

	//Calc distances and hitpoint
	auto distance = MathHelper::GetDistanceBetweenPoints(enemyCenter, targetCenter);
	*raycastHitpoint = _collisions->GetRayHitpoint(enemyCenter, angle, distance);
	auto enemyRaycastDistance = MathHelper::GetDistanceBetweenPoints(enemyCenter, *raycastHitpoint);

	//Decide if direct line of sight
	bool directHit = false;
	if (enemyRaycastDistance - rayPrecision < distance && enemyRaycastDistance + rayPrecision > distance)
		directHit = true;
	else
		directHit = false;

	//Update weapon
	auto wpn = source->GetWeapon();
	if (wpn != nullptr)
	{
		wpn->SetCurrentAngle(angle);
		wpn->SetRaycastHitpoint(*raycastHitpoint);
		if (directHit)
			wpn->SetRaycastColor(sf::Color::Yellow);
		else
			wpn->SetRaycastColor(sf::Color::Cyan);
	}

	return directHit;
}

void EnemiesAI::PrepareVertex()
{
	_pathfindLines.clear();
	_pathfindLines.resize(0);

	auto enemies = _enemies->GetEnemies();
	for (size_t i = 0; i < enemies->size(); i++)
	{
		auto e = enemies->at(i);
		auto found = _enemyPath.find(e);
		if (found == _enemyPath.end()) continue;

		sf::Vertex v;
		v.color = _pathfindLinesColor;
		if (found->second.size() > 0)
		{
			v.position = found->second.front();
			_pathfindLines.append(v);
			v.position = ViewHelper::GetRectCenter(e->GetCollisionBox());
			_pathfindLines.append(v);

			auto start = found->second.begin();
			start++;
			for (auto it = start;it!=found->second.end();it++)
			{
				v.position = *it;
				_pathfindLines.append(v);

				it--;

				v.position = *it;
				_pathfindLines.append(v);

				it++;
			}
			v.position = ViewHelper::GetRectCenter(_target->GetCollisionBox());
			_pathfindLines[_pathfindLines.getVertexCount() - 2] = v;
		}	
	}
}

EnemiesAI::EnemiesAI()
{
	_logger = Logger::GetInstance();

	_target = nullptr;
	_collisions = nullptr;
	_enemies = nullptr;

	_pathfindLines.setPrimitiveType(sf::Lines);
	_pathfindLines.resize(0);
	_pathfindLines.clear();
	_pathfindLinesColor = sf::Color::Green;
	_showPathfindLines = false;
	
	_enemyPath.clear();
	_allPaths.clear();
	_lastNeighbours.clear();
}

EnemiesAI::~EnemiesAI()
{
}

void EnemiesAI::Update(float deltaTime)
{
	if (_enemies == nullptr || _target == nullptr) return;

	//Set vars
	auto enemies = _enemies->GetEnemies();
	if (enemies->size() == 0) return;

	auto targetView = _target->GetView();
	auto acctualTargetPos = ViewHelper::GetRectCenter(_target->GetCollisionBox());

	//Check if there is need to generate new paths
	bool same = true;
	if (acctualTargetPos != _lastTargetPos)
	{
		std::unordered_map<Vector2MapKey<float>, bool, Vector2MapKeyHasher<float>> toFill;
		auto neighbours = _pathfind.GetNodesInSight(acctualTargetPos, _collisions);
		for (auto it = neighbours.begin(); it != neighbours.end(); it++)
		{
			auto key = Vector2MapKey<float>(*it);
			toFill[key] = true;
			if (_lastNeighbours[key] == false)
				same = false;
		}

		for (auto &p : _lastNeighbours)
			if (p.second == true)
				if (toFill[p.first] == false)
					same = false;

		_lastNeighbours.clear();
		_lastNeighbours = toFill;
	}
	if(same == false)
		_allPaths = _pathfind.GetDijkstrasPath(acctualTargetPos, _collisions);

	//Go through enemies
	for (size_t i = 0; i < enemies->size(); i++)
	{
		//Set vars
		auto currentEnemy = enemies->at(i);
		auto enemyBox = currentEnemy->GetCollisionBox();
		auto currentEnemyPos = ViewHelper::GetRectCenter(enemyBox);

		if (currentEnemy->IsAiEnabled() == false && CollisionHelper::CheckSimpleCollision(targetView, enemyBox) == false) //Not in player view
		{
			auto wpn = currentEnemy->GetWeapon();
			if (wpn != nullptr && wpn->GetRaycastVisibility() == true) //Hide raycasts when outside screen and not chasing
				wpn->SetRaycastHitpoint(currentEnemyPos);
			continue;
		}

		sf::Vector2f gotoPoint = currentEnemyPos;
		bool direct = DirectLineOfSight(currentEnemy, &gotoPoint);
		if (direct == false) //If no direct, find path
		{
			if (currentEnemy->IsAiEnabled() == false) continue;

			auto pathFromPaths = _enemyPath.find(currentEnemy);
			if (acctualTargetPos != _lastTargetPos && (same == false || pathFromPaths == _enemyPath.end() || pathFromPaths->second.size() == 0)) //Player moved or enemy has no path
			{
				sf::Vector2f nowGoingTo;

				//Some path exists, clear all, left only acctual point
				if (pathFromPaths != _enemyPath.end() && pathFromPaths->second.size() > 0) 
					nowGoingTo = pathFromPaths->second.front();
				else //Find first point
					nowGoingTo = _pathfind.GetClosestVisibleNodeTo(_allPaths, currentEnemyPos, acctualTargetPos, _collisions);
				_enemyPath[currentEnemy].clear();

				//Get path from enemy to player
				sf::Vector2f* currPoint = nullptr;
				currPoint = (nowGoingTo.x == INFINITY) ? nullptr : &nowGoingTo;
				while (currPoint != nullptr)
				{
					_enemyPath[currentEnemy].push_back(*currPoint);

					auto found = _allPaths.find(Vector2MapKey<float>(*currPoint));
					if (found == _allPaths.end()) break;

					if (found->second.x == INFINITY) currPoint = nullptr;
					else currPoint = &found->second;
				}

				if (_enemyPath[currentEnemy].size() == 0) //If no path, nor direct, exit
				{
					if (currentEnemy->IsAttacking() == false)
						currentEnemy->SetState("idle");
					currentEnemy->SetAI(false);
					continue;
				}
				else
				{
					gotoPoint = _enemyPath[currentEnemy].front();
				}
			}
			else
			{
				if(pathFromPaths != _enemyPath.end() && pathFromPaths->second.size() > 0)
					gotoPoint = pathFromPaths->second.front();
				else
				{
					if (currentEnemy->IsAttacking() == false)
						currentEnemy->SetState("idle");
					currentEnemy->SetAI(false);
					continue;
				}
			}
		}
		else
		{
			currentEnemy->SetAI(true);
			auto found = _enemyPath.find(currentEnemy);
			if (found != _enemyPath.end())
				_enemyPath.erase(found);
		}
		
		if (CollisionHelper::CheckSimpleCollision(_target->GetCollisionBox(), currentEnemy->GetCollisionBox()) == false) //Not touching
		{
			auto startBox = currentEnemy->GetCollisionBox();
			MoveStraightToPoint(currentEnemy, gotoPoint, deltaTime);

			//For collision checking
			auto offset = currentEnemy->GetCollisionBoxOffset();
			auto offsetPos = sf::Vector2f(offset.left, offset.top);
			auto endBox = currentEnemy->GetCollisionBox();

			//Check collisions with walls
			auto pos = _collisions->GetLimitPosition(startBox, endBox);
			currentEnemy->SetPosition(pos - offsetPos);

			//Check collisions with other enemies
			for (size_t no = 0; no < enemies->size(); no++)
			{
				if (no == i) continue;

				auto checkedEnemy = enemies->at(no);
				if (CollisionHelper::CheckSimpleCollision(currentEnemy->GetCollisionBox(), checkedEnemy->GetCollisionBox()))
				{
					auto newPos = CollisionHelper::GetRectLimitPosition(startBox, endBox, checkedEnemy->GetCollisionBox());
					currentEnemy->SetPosition(newPos - offsetPos);
				}
			}

			//If reached point, remove it, to go to the next
			if (direct == false && gotoPoint == ViewHelper::GetRectCenter(currentEnemy->GetCollisionBox()))
				if(_enemyPath[currentEnemy].size() > 0)
					_enemyPath[currentEnemy].pop_front();

			/* May be used to regroup
			float radius = MathHelper::GetDistanceBetweenPoints(collisionBoxCenter, sf::Vector2f(collisionBox.left, collisionBox.top));
			for (auto& e : _collisions->GetEdges())
			{
				for (unsigned char i = 0; i < 2; i++)
				{
					sf::Vector2f p;
					p = (i == 0) ? std::get<0>(e) : std::get<1>(e);
					if (CollisionHelper::CheckCircleCollision(p, collisionBoxCenter, radius, 180.F, 0))
					{
						float angle = MathHelper::GetAngleBetweenPoints(collisionBoxCenter, p);
						auto point = MathHelper::GetPointFromAngle(collisionBoxCenter, angle, radius);
						sf::Vector2f moveVector(p - point);
						source->SetPosition(source->GetPosition() + moveVector);
					}
				}
			}*/
		}

		if (currentEnemy->IsAttacking() == false)
			currentEnemy->SetState("move");
	}

	if(acctualTargetPos != _lastTargetPos)
		_lastTargetPos = acctualTargetPos;

	if (_showPathfindLines)
		PrepareVertex();
}

void EnemiesAI::ClearEnemiesPaths()
{
	_enemyPath.clear();
	_lastTargetPos = sf::Vector2f(-1, -1);
}

void EnemiesAI::MoveStraightToPoint(Enemy* source, sf::Vector2f point, float deltaTime)
{
	if (source == nullptr) return;

	auto srcCenter = ViewHelper::GetRectCenter(source->GetCollisionBox());
	sf::Vector2f moveVector(point - srcCenter);

	auto distance = MathHelper::GetDistanceBetweenPoints(srcCenter, point);
	auto radius = source->GetStep() * deltaTime * source->GetSpeed();
	auto angle = MathHelper::GetAngleBetweenPoints(srcCenter, point);
	auto move = MathHelper::GetPointFromAngle(srcCenter, angle, radius);

	if (distance > radius)
		moveVector = move - srcCenter;

	if (moveVector.x < 0) source->GetAnimations()->ApplySetHorizontalFlip(true);
	else if(moveVector.x > 0) source->GetAnimations()->ApplySetHorizontalFlip(false);

	source->SetPosition(source->GetPosition() + moveVector);
}

void EnemiesAI::SetPathfindVisibility(bool visible)
{
	_showPathfindLines = visible;
}

void EnemiesAI::SetPathfindColor(sf::Color color)
{
	for (size_t i = 0; i < _pathfindLines.getVertexCount(); i++)
		_pathfindLines[i].color = color;
	_pathfindLinesColor = color;
}

bool EnemiesAI::GetPathfindVisibility()
{
	return _showPathfindLines;
}

sf::Color EnemiesAI::GetPathfindColor()
{
	return _pathfindLinesColor;
}

void EnemiesAI::TogglePathfindingVisibility()
{
	std::string status = (!_showPathfindLines) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show pathfinding lines: " + status);
	SetPathfindVisibility(!_showPathfindLines);
}

void EnemiesAI::SetTarget(Entity* target)
{
	_target = target;
}

void EnemiesAI::SetEnemiesManager(EnemiesManager* manager)
{
	_enemies = manager;
}

void EnemiesAI::SetCollisionsManager(CollisionsManager* manager)
{
	_collisions = manager;
}

void EnemiesAI::SetPathfindPoints(std::vector<sf::Vector2f> points)
{
	_pathfind.GenerateBaseGraph(points, _collisions);
}

void EnemiesAI::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if(_showPathfindLines)
		target.draw(_pathfindLines);
}
