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

EnemiesAI::EnemiesAI()
{
	_target = nullptr;
	_collisions = nullptr;
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
		sf::Vector2f raycastHitpoint;
		if (DirectLineOfSight(currentEnemy, &raycastHitpoint))
		{
			if (CollisionHelper::CheckSimpleCollision(_target->GetCollisionBox(), currentEnemy->GetCollisionBox()) == false)
			{
				auto startBox = currentEnemy->GetCollisionBox();
				MoveStraightToPoint(currentEnemy, raycastHitpoint, deltaTime);

				//Check collisions with walls
				auto endBox = currentEnemy->GetCollisionBox();
				auto pos = _collisions->GetLimitPosition(startBox, endBox);
				auto offset = currentEnemy->GetCollisionBoxOffset();
				currentEnemy->SetPosition(pos.x - offset.left, pos.y - offset.top);

				//Check collisions with other enemies
				for (size_t no = 0; no < enemies->size(); no++)
				{
					if (no == i) continue;

					auto checkedEnemy = enemies->at(no);
					if (CollisionHelper::CheckSimpleCollision(currentEnemy->GetCollisionBox(), checkedEnemy->GetCollisionBox()))
					{
						auto newPos = CollisionHelper::GetRectLimitPosition(startBox, endBox, checkedEnemy->GetCollisionBox());
						currentEnemy->SetPosition(newPos.x - offset.left, newPos.y - offset.top);
					}
				}
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
		else
			if (currentEnemy->IsAttacking() == false)
				currentEnemy->SetState("idle");
	}
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
