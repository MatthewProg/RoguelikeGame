#include "EnemiesManager.h"

EnemiesManager::EnemiesManager()
{
	_enemies.clear();
	_logger = Logger::GetInstance();
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

		it->Update(tick);
	}
}

void EnemiesManager::CheckForHit(Player* player)
{
	auto weapon = player->GetWeapon();
	auto playerCenter = ViewHelper::GetRectCenter(player->GetCollisionBox());

	for (auto obj : _enemies)
	{
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
