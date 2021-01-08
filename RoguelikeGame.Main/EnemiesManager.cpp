#include "EnemiesManager.h"

EnemiesManager::EnemiesManager()
{
	_enemies.clear();
	_logger = Logger::GetInstance();
	_player = nullptr;
	_collisionsManager = nullptr;
}

EnemiesManager::~EnemiesManager()
{
	for (auto it : _enemies)
		delete it;
}

void EnemiesManager::Update(bool tick, float deltaTime)
{
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		auto it = _enemies[i];

		if (tick) //No need to do it every frame
		{
			if (it->IsDead())
			{
				_enemies.erase(_enemies.begin() + i);
				break;
			}
		}

		it->Update(tick, deltaTime);

		auto wpn = it->GetWeapon();
		if (wpn == nullptr) continue;

		wpn->setPosition(ViewHelper::GetRectCenter(it->GetCollisionBox()));
	}
}

void EnemiesManager::CheckForHit()
{
	//Player -> Enemy
	auto weapon = _player->GetWeapon();
	auto playerCenter = ViewHelper::GetRectCenter(_player->GetCollisionBox());

	for (auto obj : _enemies)
	{
		if (weapon == nullptr) continue;

		if (weapon->GetWeaponType() == WeaponType::MELEE)
		{
			auto enemyHitboxPoints = CollisionHelper::GetRectPoints(obj->GetCollisionBox());
			MeleeWeapon* wep = (MeleeWeapon*)weapon;
			for (auto p : enemyHitboxPoints)
			{
				if (CollisionHelper::CheckCircleCollision(p, playerCenter, wep->GetWeaponRange(), wep->GetWeaponAngle(), wep->GetCurrentAngle()))
				{
					obj->TakeDmg(wep->GetWeaponDMG());
					break;
				}
			}
		}
	}
}

void EnemiesManager::CheckAttacks()
{
	//Enemy -> Player
	auto playerHitbox = _player->GetCollisionBox();

	for (auto enemy : _enemies)
	{
		auto enemyWeapon = enemy->GetWeapon();
		if (enemyWeapon == nullptr) continue;

		if (enemyWeapon->CanAttack() == false)
			continue;

		if (enemyWeapon->GetWeaponType() == WeaponType::NONE) //Enemy attacks if hitboxes collide
		{
			auto enemyHitbox = enemy->GetCollisionBox();
			if (CollisionHelper::CheckSimpleCollision(enemyHitbox, playerHitbox))
			{
				_player->TakeDmg(enemyWeapon->GetWeaponDMG());
				enemy->Attack();
				continue;
			}
		}
	}
}

void EnemiesManager::UpdateRays()
{
	auto plrCenter = ViewHelper::GetRectCenter(_player->GetCollisionBox());
	auto rayPrecision = 2.F;

	for (auto& enemy : _enemies)
	{
		auto wpn = enemy->GetWeapon();
		if (wpn != nullptr)
		{
			auto enemyCenter = ViewHelper::GetRectCenter(enemy->GetCollisionBox());
			auto angle = MathHelper::GetAngleBetweenPoints(enemyCenter, plrCenter);
			auto distance = MathHelper::GetDistanceBetweenPoints(enemyCenter, plrCenter);
			auto raycastHitpoint = _collisionsManager->GetRayHitpoint(enemyCenter, angle, distance);
			auto enemyRaycastDistance = MathHelper::GetDistanceBetweenPoints(enemyCenter, raycastHitpoint);

			wpn->SetRaycastHitpoint(raycastHitpoint);
			if (enemyRaycastDistance - rayPrecision < distance && enemyRaycastDistance + rayPrecision > distance)
				wpn->SetRaycastColor(sf::Color::Yellow);
			else
				wpn->SetRaycastColor(sf::Color::Cyan);
		}
	}
}

void EnemiesManager::SetPlayer(Player* player)
{
	_player = player;
}

void EnemiesManager::SetCollisionsManager(CollisionsManager* manager)
{
	_collisionsManager = manager;
}

void EnemiesManager::SetEnemiesHitboxVisibility(bool visibility)
{
	for (auto it : _enemies)
		it->SetHitboxVisibility(visibility);
}

bool EnemiesManager::GetEnemiesHitboxVisibility()
{
	if(_enemies.size() > 0)
		return _enemies[0]->GetHitboxVisibility();
	return false;
}

void EnemiesManager::ToggleEnemiesHitboxVisibility()
{
	std::string status = (!GetEnemiesHitboxVisibility()) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show enemies hitbox: " + status);
	SetEnemiesHitboxVisibility(!GetEnemiesHitboxVisibility());
}

void EnemiesManager::ToggleEnemiesRaycastVisibility()
{
	bool status = false;
	bool logged = false;

	for (auto& enemy : _enemies)
	{
		auto enemyWeapon = enemy->GetWeapon();
		if (enemyWeapon != nullptr)
		{
			if (logged == false)
			{
				status = enemyWeapon->GetRaycastVisibility();
				std::string mess = (!status) ? "true" : "false";
				_logger->Log(Logger::LogType::INFO, "Show enemies weapon raycast: " + mess);
				logged = true;
			}
			enemyWeapon->SetRaycastVisibility(!status);
		}
	}
}

void EnemiesManager::Add(Enemy* enemy)
{
	_enemies.push_back(enemy);
}

std::vector<Enemy*>* EnemiesManager::GetEnemies()
{
	return &_enemies;
}

void EnemiesManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it : _enemies)
		target.draw(*it);
}
