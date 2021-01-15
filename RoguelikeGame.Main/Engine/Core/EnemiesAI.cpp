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
			v.position = ViewHelper::GetRectCenter(e->GetCollisionBox());
			_pathfindLines.append(v);
			v.position = found->second[found->second.size() - 1];
			_pathfindLines.append(v);

			for (size_t j = found->second.size() - 1; j > 0; j--)
			{
				v.position = found->second[j];
				_pathfindLines.append(v);

				v.position = found->second[j - 1];
				_pathfindLines.append(v);
			}
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
}

EnemiesAI::~EnemiesAI()
{
}

void EnemiesAI::Update(float deltaTime)
{
	if (_enemies == nullptr || _target == nullptr) return;

	auto enemies = _enemies->GetEnemies();
	for (size_t i = 0; i < enemies->size(); i++)
	{
		auto currentEnemy = enemies->at(i);
		auto acctualTargetPos = ViewHelper::GetRectCenter(_target->GetCollisionBox());
		auto currentEnemyPos = ViewHelper::GetRectCenter(currentEnemy->GetCollisionBox());

		sf::Vector2f gotoPoint;
		bool direct = DirectLineOfSight(currentEnemy, &gotoPoint);
		if (direct == false) //If no direct, find path
		{
			if (acctualTargetPos != _lastTargetPos)
			{
				_enemyPath[currentEnemy] = _pathfind.GetPath(acctualTargetPos, currentEnemyPos , _collisions);
				if (_enemyPath[currentEnemy].size() == 0) //If no path, nor direct, exit
				{
					if (currentEnemy->IsAttacking() == false)
						currentEnemy->SetState("idle");
					continue;
				}
				else
				{
					gotoPoint = _enemyPath[currentEnemy].at(_enemyPath[currentEnemy].size() - 1);
				}
			}
		}
		else
		{
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
				_enemyPath[currentEnemy].pop_back();

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

	auto move = source->GetStep() * deltaTime * source->GetSpeed();
	if (abs(moveVector.x) > move)
		moveVector.x = (moveVector.x < 0) ? move * (-1) : move;
	if (abs(moveVector.y) > move)
		moveVector.y = (moveVector.y < 0) ? move * (-1) : move;

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

void EnemiesAI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(_showPathfindLines)
		target.draw(_pathfindLines);
}
